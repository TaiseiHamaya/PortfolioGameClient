#include "ZoneHandler.h"

#include <Engine/Application/Output.h>

#include "./Command/ZoneLoginPlayerCommand.h"
#include "./Command/ZoneLogoutPlayerCommand.h"
#include "./Command/ZoneSyncTransformCommand.h"

#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"
#include "Scripts/Network/GameServer/GameServerPacketReceiver.h"
#include "Scripts/Network/GameServer/GameServerPacketSender.h"

void ZoneHandler::setup(Reference<EntityManager> entityManager_, Reference<GameServerConnectionManager> gameServerConnectionManager_, Reference<GameServerPacketReceiver> gameServerPacketReceiver_, Reference<GameServerPacketSender> gameServerPacketSender_) {
	entityManager = entityManager_;
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
		Information("Recv packet byte size long {}", packet.ByteSizeLong());

		switch (packet.category_case()) {
		case Proto::Packet::kTextMessageType:
			// TextMessageTypeの処理
			process_text_message(packet.textmessagetype(), packet.payload());
			break;
		case Proto::Packet::kLoginPacketType:
			// LoginPacketTypeの処理
			process_login_packet(packet.loginpackettype(), packet.payload());
			break;
		case Proto::Packet::kLogoutPacketType:
			// LogoutPacketTypeの処理
			process_logout_packet(packet.logoutpackettype(), packet.payload());
			break;
		case Proto::Packet::kSyncPacketType:
			// SyncPacketTypeの処理
			process_sync_packet(packet.syncpackettype(), packet.payload());
			break;
		default:
			Warning("");
			// その他の処理
			break;
		}
	}
}

void ZoneHandler::stack_command(std::unique_ptr<IZoneCommand> command) {
	zoneCommands.emplace_back(std::move(command));
}

void ZoneHandler::move_client_player(const Vector3& position) {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
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
	packet.set_syncpackettype(Proto::SyncTransform);
	Proto::TransformSyncBody body;
	body.set_id(serverId.value());
	body.set_timestamp(now.time_since_epoch().count());
	Proto::Vector3* pos = body.mutable_position();
	pos->set_x(position.x);
	pos->set_y(position.y);
	pos->set_z(position.z);
	packet.set_payload(body.SerializeAsString());
	gameServerPacketSender->stack_packet(packet);
}

void ZoneHandler::process_text_message(Proto::TextMessageType type, const std::string& payload) {
	switch (type) {
	case Proto::MessageChatReceive:
	{
		Proto::ChatMessageBody body;
		body.ParseFromString(payload);
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		if (entity) {
			Information("[{}]: {}", entity->name_imu(), body.message());
		}
		else {
			Information("[Unknown]: {}", body.message());
		}
	}
	break;
	case Proto::MessageSystemMessage:
	{
		Proto::SystemMessageBody body;
		body.ParseFromString(payload);
		Information("[System]: {}", body.message());
	}
	break;
	}
}

void ZoneHandler::process_login_packet(Proto::LoginPacketType type, const std::string& payload) {
	switch (type) {
	case Proto::LoginResult:
	{
		Proto::LoginResultBody body;
		body.ParseFromString(payload);
		gameServerConnectionManager->on_connection_succeeded();
		player->set_server_id(body.id());
		player->set_name(body.username());
		player->get_transform().set_translate(Vector3{ body.position().x(), body.position().y(), body.position().z() });
		entityManager->register_server_id(body.id(), player);
	}
	break;
	case Proto::LoginNotification:
	{
		Proto::LoginNotificationBody body;
		body.ParseFromString(payload);
		Information("[System]: Player added. Id-\'{}\' Name-\'{}\'", body.id(), body.username());
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

void ZoneHandler::process_logout_packet(Proto::LogoutPacketType type, const std::string& payload) {
	switch (type) {
	case Proto::LogoutResponse:
	{
		Proto::LogoutResponseBody body;
		body.ParseFromString(payload);
	}
	break;
	case Proto::LogoutNotification:
	{
		Proto::LogoutNotificationBody body;
		body.ParseFromString(payload);
		zoneCommands.emplace_back(
			std::make_unique<ZoneLogoutPlayerCommand>(entityManager, body.id())
		);
		Information("[System]: Player removed. Id-\'{}\'", body.id());
	}
	break;
	default:
		break;
	}
}

void ZoneHandler::process_sync_packet(Proto::SyncPacketType type, const std::string& payload) {
	switch (type) {
	case Proto::SyncTransform:
	{
		Proto::TransformSyncBody body;
		body.ParseFromString(payload);
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		using time_point = std::chrono::steady_clock::time_point;
		time_point time{ std::chrono::duration_cast<time_point::duration>(std::chrono::microseconds(body.timestamp())) };

		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		zoneCommands.emplace_back(
			std::make_unique<ZoneSyncTransformCommand>(entity, time, position)
		);
	}
	break;
	default:
		break;
	}
}

void ZoneHandler::set_player(Reference<Player> player_) {
	player = player_;
}
