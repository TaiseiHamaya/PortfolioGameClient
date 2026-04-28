#include "ZoneHandler.h"

#include <Engine/Application/Logger.h>
#include <Engine/Module/World/WorldInstance/WorldInstance.h>

#include "./Command/ZonePlayActionCommand.h"
#include "./Command/ZoneSyncTransformCommand.h"

#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Network/NetworkCluster.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"
#include "Scripts/Network/GameServer/GameServerPacketReceiver.h"
#include "Scripts/Network/GameServer/GameServerPacketSender.h"

#include "process/gateway/packet.pb.h"

void ZoneHandler::setup(Reference<EntityManager> entityManager_, Reference<EnemyManager> enemyManager_, Reference<GameLogWindowManager> gameLogWindowManager_) {
	entityManager = entityManager_;
	enemyManager = enemyManager_;
	gameServerConnectionManager = NetworkCluster::ConnectionManagerMut();
	gameServerPacketReceiver = NetworkCluster::ReceiverMut();
	gameServerPacketSender = NetworkCluster::SenderMut();
	gameLogWindowManager = gameLogWindowManager_;

	zone.set_entity_manager(entityManager_);
	zone.set_enemy_manager(enemyManager_);

	auto commandStack = [this](std::unique_ptr<IZoneCommand> cmd) { stack_command(std::move(cmd)); };
	textMessageHandler.setup(entityManager_, gameLogWindowManager_);
	loginMessageHandler.setup(entityManager_, gameLogWindowManager_, commandStack);
	logoutMessageHandler.setup(entityManager_, gameLogWindowManager_, commandStack);
	syncMessageHandler.setup(entityManager_, gameLogWindowManager_, commandStack);
	enemyMessageHandler.setup(enemyManager_, entityManager_);

	router.register_handler(Proto::ToClientMessage::kTextMessage, std::ref(textMessageHandler));
	router.register_handler(Proto::ToClientMessage::kStartGameResponse, std::ref(loginMessageHandler));
	router.register_handler(Proto::ToClientMessage::kLogoutResponse, std::ref(logoutMessageHandler));
	router.register_handler(Proto::ToClientMessage::kZoneEnterNotification, std::ref(loginMessageHandler));
	router.register_handler(Proto::ToClientMessage::kZoneExitNotification, std::ref(logoutMessageHandler));
	router.register_handler(Proto::ToClientMessage::kTransformSync, std::ref(syncMessageHandler));
	router.register_handler(Proto::ToClientMessage::kPlayAction, std::ref(syncMessageHandler));
	router.register_handler(Proto::ToClientMessage::kEntityDamaged, std::ref(syncMessageHandler));
	router.register_handler(Proto::ToClientMessage::kEnemySpawn, std::ref(enemyMessageHandler));
	router.register_handler(Proto::ToClientMessage::kEnemyDespawn, std::ref(enemyMessageHandler));

	chatBoxManager.initialize();

	Proto::ToServerMessage enter;
	Proto::PayloadLobbyStartGameRequest* payload = enter.mutable_start_game();
	NetworkCluster::SenderMut()->stack_packet(enter);

	NetworkCluster::Send();
}

void ZoneHandler::prev_update() {
	NetworkCluster::Receive();

	handle_zone();

	chatBoxManager.update();
	if (chatBoxManager.is_enter_frame()) {
		std::wstring message = chatBoxManager.into_string();
		szgInformation(L"Send chat message: {}", message);

		if (gameServerConnectionManager && gameServerConnectionManager->is_established() && gameServerPacketSender && player) {
			const std::optional<u64>& serverId = player->server_id();
			if (serverId.has_value()) {
				// パケット作成
				Proto::ToServerMessage packet;
				Proto::PayloadTextMessage body;
				body.set_id(serverId.value());
				body.set_message(ConvertString(message));
				packet.set_allocated_text_message(&body);
				// 送信
				gameServerPacketSender->stack_packet(packet);
			}
		}
	}

	execute_commands();
}

void ZoneHandler::post_update() {
	NetworkCluster::Send();
}

void ZoneHandler::finalize() {
	Proto::ToServerMessage enter;
	Proto::PayloadLobbyEndGameRequest* payload = enter.mutable_end_game();
	NetworkCluster::SenderMut()->stack_packet(enter);

	NetworkCluster::Send();
}

void ZoneHandler::execute_commands() {
	for (auto& command : zoneCommands) {
		command->execute(zone);
	}
	zoneCommands.clear();
}	

void ZoneHandler::handle_zone() {
	if (!gameServerPacketReceiver || !player) {
		return;
	}

	auto packets = gameServerPacketReceiver->take_packet_stack();

	router.dispatch(packets);

#ifdef DEBUG_FEATURES_ENABLE
	debugRecivedMessageCount = static_cast<i32>(packets.size());
	debugCommandCount = static_cast<i32>(zoneCommands.size());
	debugSentMessageCount = gameServerPacketSender ? static_cast<i32>(gameServerPacketSender->packet_count()) : 0;
#endif // DEBUG_FEATURES_ENABLE
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
	if (!gameServerConnectionManager || !gameServerConnectionManager->is_established()) {
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
	Proto::ToServerMessage packet;
	Proto::PayloadTransformSync* payload = packet.mutable_transform_sync();
	payload->set_id(serverId.value());
	payload->set_timestamp(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
	// 位置の書き込み
	Proto::Vector3* pos = payload->mutable_position();
	pos->set_x(position.x);
	pos->set_y(position.y);
	pos->set_z(position.z);
	gameServerPacketSender->stack_packet(packet);
}

void ZoneHandler::request_play_action(u32 actionId) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	// TODO : 要修正
	bool result = player->can_play_action("PaladinHolySpirit"_sh);
	if (!result) {
		szgWarning("Failed to start action Id:{}", actionId);
		return;
	}
	// グローバルスキルの処理
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
	if (!gameServerConnectionManager || !gameServerConnectionManager->is_established()) {
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
	Proto::ToServerMessage packet;
	Proto::PayloadPlayAction body;
	body.set_id(serverId.value());
	body.set_timestamp(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());
	body.set_action_id(actionId);
	body.set_target_id(target->server_id().value());
	packet.set_allocated_play_action(&body);
	gameServerPacketSender->stack_packet(packet);
}

Reference<const ChatBoxManager> ZoneHandler::chat_box_imm() const noexcept {
	return chatBoxManager;
}

void ZoneHandler::set_player(Reference<Player> player_) {
	player = player_;
	zone.set_player(player_);
	loginMessageHandler.set_player(player_);
	syncMessageHandler.set_player(player_);
	enemyMessageHandler.set_player(player_);
}

void ZoneHandler::set_effect_manager(Reference<EffectManager> effectManager_) {
	zone.set_effect_manager(effectManager_);
}

void ZoneHandler::set_camera_instance(Reference<const szg::WorldInstance> cameraInstance_) {
	zone.set_camera_instance(cameraInstance_);
}

#ifdef DEBUG_FEATURES_ENABLE

void ZoneHandler::debug_gui() {
	ImGui::Text(std::format("SentMessageCount-\'{}\'", debugSentMessageCount).c_str());
	ImGui::Text(std::format("ReceivedMessageCount-\'{}\'", debugRecivedMessageCount).c_str());
	ImGui::Text(std::format("CommandCount-\'{}\'", debugCommandCount).c_str());

	ImGui::Separator();

	player->debug_gui();

	gameServerConnectionManager->debug_gui();

	if (!player) {
		return;
	}

	const auto& id = player->server_id();
	if (!id.has_value()) {
		return;
	}
	ImGui::Separator();

	ImGui::Text(player->name_imm().c_str());

	bool send = false;
	send |= ImGui::InputText("Chat", &msg, ImGuiInputTextFlags_EnterReturnsTrue);
	send |= ImGui::Button("SendMessage");
	if (send) {
		Proto::ToServerMessage packet;
		Proto::PayloadTextMessage* body = packet.mutable_text_message();
		body->set_message(msg);
		body->set_id(id.value());


		gameServerPacketSender->stack_packet(packet);
		msg.clear();
	}
}

#endif // DEBUG_FEATURES_ENABLE
