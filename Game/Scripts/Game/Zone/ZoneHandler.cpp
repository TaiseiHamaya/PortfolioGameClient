#include "ZoneHandler.h"

#include <Engine/Application/Logger.h>

#include "./Command/ZoneEntityDamagedCommand.h"
#include "./Command/ZoneLoginPlayerCommand.h"
#include "./Command/ZoneLogoutPlayerCommand.h"
#include "./Command/ZonePlayActionCommand.h"
#include "./Command/ZoneSyncTransformCommand.h"

#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"
#include "Scripts/Network/GameServer/GameServerPacketReceiver.h"
#include "Scripts/Network/GameServer/GameServerPacketSender.h"

void ZoneHandler::setup(Reference<EntityManager> entityManager_, Reference<EnemyManager> enemyManager_, Reference<GameServerConnectionManager> gameServerConnectionManager_, Reference<GameServerPacketReceiver> gameServerPacketReceiver_, Reference<GameServerPacketSender> gameServerPacketSender_) {
	entityManager = entityManager_;
	enemyManager = enemyManager_;
	gameServerConnectionManager = gameServerConnectionManager_;
	gameServerPacketReceiver = gameServerPacketReceiver_;
	gameServerPacketSender = gameServerPacketSender_;
}

void ZoneHandler::execute_commands() {
	for (auto& command : zoneCommands) {
		command->execute();
	}
	zoneCommands.clear();
}

void ZoneHandler::handle_zone() {
	if (!gameServerPacketReceiver) {
		return;
	}

	auto packets = gameServerPacketReceiver->take_packet_stack();

	for (auto& packet : packets) {
		szgInformation("Recv packet byte size long {}", packet.ByteSizeLong());

		switch (packet.category_case()) {
		case Proto::Packet::kCategoryTextMessage:
			// TextMessageTypeの処理
			process_text_message(packet.category_text_message(), packet.payload());
			break;
		case Proto::Packet::kCategoryLoginMessage:
			// LoginPacketTypeの処理
			process_login_message(packet.category_login_message(), packet.payload());
			break;
		case Proto::Packet::kCategoryLogoutMessage:
			// LogoutPacketTypeの処理
			process_logout_message(packet.category_logout_message(), packet.payload());
			break;
		case Proto::Packet::kCategorySyncMessage:
			// SyncPacketTypeの処理
			process_sync_message(packet.category_sync_message(), packet.payload());
			break;
		case Proto::Packet::kCategoryEnemyMessage:
			// EntityPacketTypeの処理
			process_entity_message(packet.category_enemy_message(), packet.payload());
			break;
		default:
			szgError("Unknown packet category.");
			// その他の処理
			break;
		}
	}
}

void ZoneHandler::stack_command(std::unique_ptr<IZoneCommand> command) {
	zoneCommands.emplace_back(std::move(command));
}

void ZoneHandler::move_client_player(const Vector3& position) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	// コマンド追加
	zoneCommands.emplace_back(
		std::make_unique<ZoneSyncTransformCommand>(
			player,
			now,
			position
		)
	);
	if (!gameServerConnectionManager || !gameServerConnectionManager->is_connected()) {
		return;
	}
	if (!gameServerPacketSender) {
		return;
	}
	const std::optional<u64>& serverId = player->server_id();
	if (!serverId.has_value()) {
		return;
	}
	// パケット作成
	Proto::Packet packet;
	packet.set_category_sync_message(Proto::SYNC_TRANSFORM);
	Proto::PayloadTransformSync body;
	body.set_id(serverId.value());
	body.set_timestamp(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
	Proto::Vector3* pos = body.mutable_position();
	pos->set_x(position.x);
	pos->set_y(position.y);
	pos->set_z(position.z);
	packet.set_payload(body.SerializeAsString());
	gameServerPacketSender->stack_packet(packet);
}

void ZoneHandler::request_play_action(u32 actionId) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	bool result = player->can_play_action("PaladinHolySpirit"_sh);
	if (!result) {
		szgInformation("Failed to start action Id:{}", actionId);
		return;
	}
	bool isGlobalSkill = player->is_global_skill("PaladinHolySpirit"_sh);
	if (isGlobalSkill) {
		player->execute_global_skill();
	}
	// コマンド追加
	zoneCommands.emplace_back(
		std::make_unique<ZonePlayActionCommand>(
			player,
			actionId,
			player->get_selection_target()
		)
	);
	if (!gameServerConnectionManager || !gameServerConnectionManager->is_connected()) {
		return;
	}
	if (!gameServerPacketSender) {
		return;
	}
	const std::optional<u64>& serverId = player->server_id();
	if (!serverId.has_value()) {
		return;
	}
	Reference<IEntity> target = player->get_selection_target();
	if (!target) {
		return;
	}
	// パケット作成
	// アクション実行
	Proto::Packet packet;
	packet.set_category_sync_message(Proto::CategorySyncMessage::PLAY_ACTION);
	Proto::PayloadPlayAction body;
	body.set_id(serverId.value());
	body.set_timestamp(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
	body.set_action_id(actionId);
	body.set_entity_id(target->server_id().value());
	packet.set_payload(body.SerializeAsString());
	gameServerPacketSender->stack_packet(packet);
}

void ZoneHandler::process_text_message(Proto::CategoryTextMessage type, const std::string& payload) {
	switch (type) {
	case Proto::CHAT_RECEIVE:
	{
		Proto::PayloadTextMessage body;
		body.ParseFromString(payload);
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		if (entity) {
			szgInformation("[{}]: {}", entity->name_imm(), body.message());
		}
		else {
			szgInformation("[Unknown]: {}", body.message());
		}
	}
	break;
	case Proto::SYSTEM_MESSAGE:
	{
		Proto::PayloadSystemMessage body;
		body.ParseFromString(payload);
		szgInformation("[System]: {}", body.message());
	}
	break;
	}
}

void ZoneHandler::process_login_message(Proto::CategoryLoginMessage type, const std::string& payload) {
	switch (type) {
	case Proto::LOGIN_RESULT:
	{
		Proto::PayloadLoginResult body;
		body.ParseFromString(payload);
		gameServerConnectionManager->on_connection_succeeded();
		player->set_server_id(body.id());
		player->set_name(body.username());
		player->get_transform().set_translate(Vector3{ body.position().x(), body.position().y(), body.position().z() });
		entityManager->register_server_id(body.id(), player);
		szgInformation("Login succeeded. Id-\'{}\' Name-\'{}\'", body.id(), body.username());
	}
	break;
	case Proto::LOGIN_NOTIFICATION:
	{
		Proto::PayloadLoginNotification body;
		body.ParseFromString(payload);
		szgInformation("Player added. Id-\'{}\' Name-\'{}\'", body.id(), body.username());
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		zoneCommands.emplace_back(
			std::make_unique<ZoneLoginPlayerCommand>(entityManager, body.id(), body.username(), position)
		);
	}
	break;
	default:
		break;
	}
}

void ZoneHandler::process_logout_message(Proto::CategoryLogoutMessage type, const std::string& payload) {
	switch (type) {
	case Proto::LOGOUT_RESPONSE:
	{
		Proto::PayloadLogoutResponse body;
		body.ParseFromString(payload);
	}
	break;
	case Proto::LOGOUT_NOTIFICATION:
	{
		Proto::PayloadLogoutNotification body;
		body.ParseFromString(payload);
		zoneCommands.emplace_back(
			std::make_unique<ZoneLogoutPlayerCommand>(entityManager, body.id())
		);
		szgInformation("Player removed. Id-\'{}\'", body.id());
	}
	break;
	default:
		break;
	}
}

void ZoneHandler::process_sync_message(Proto::CategorySyncMessage type, const std::string& payload) {
	switch (type) {
	case Proto::SYNC_TRANSFORM:
	{
		Proto::PayloadTransformSync body;
		body.ParseFromString(payload);
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		using time_point = std::chrono::system_clock::time_point;
		time_point time{ std::chrono::duration_cast<time_point::duration>(std::chrono::microseconds(body.timestamp())) };

		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		zoneCommands.emplace_back(
			std::make_unique<ZoneSyncTransformCommand>(entity, time, position)
		);
	}
	break;
	case Proto::PLAY_ACTION:
	{
		Proto::PayloadPlayAction body;
		body.ParseFromString(payload);
		// 自分自身のアクションは処理しない
		if (player->server_id().has_value() && body.id() == player->server_id().value()) {
			break;
		}
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		u64 targetId = body.entity_id();
		Reference<IEntity> target = entityManager->inquire_server_id(targetId);
		zoneCommands.emplace_back(
			std::make_unique<ZonePlayActionCommand>(entity, body.action_id(), target)
		);
		szgInformation("Entity Id:{} Play Action Id:{}", body.id(), body.action_id());
	}
	break;
	case Proto::ENTITY_DAMAGED:
	{
		Proto::PayloadEntityDamaged body;
		body.ParseFromString(payload);
		Reference<IEntity> target = entityManager->inquire_server_id(body.target_id());
		Reference<IEntity> attacker = entityManager->inquire_server_id(body.attacker_id());
		zoneCommands.emplace_back(
			std::make_unique<ZoneEntityDamagedCommand>(target, body.damage())
		);
		szgInformation("Entity Id:{} Damaged by Id:{} Damage:{}", body.target_id(), body.attacker_id(), body.damage());
	}
	default:
		break;
	}
}

void ZoneHandler::process_entity_message(Proto::CategoryEnemyMessage category, const std::string& payload) {
	switch (category) {
	case Proto::ENEMY_SPAWN:
	{
		// EnemyManagerに通知
		Proto::PayloadEnemySpawn body;
		body.ParseFromString(payload);
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		enemyManager->generate(std::format("{}.json", body.name()), position, body.id());
		Reference<IEntity> enemy = entityManager->inquire_server_id(body.id());
		if (enemy) {
			szgInformation("Enemy Spawned. Id-\'{}\' Name-\'{}\'", body.id(), enemy->name_imm());
		}
		player->set_target(enemy);
	}
	break;
	case Proto::ENEMY_DESPAWN:
		break;
	default:
		break;
	}
}

void ZoneHandler::set_player(Reference<Player> player_) {
	player = player_;
}
