#include "ZoneStartGameMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneEnemySpawnCommand.h"
#include "Scripts/Game/Zone/Command/ZoneLoginPlayerCommand.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"
#include "Scripts/Network/NetworkCluster.h"

void ZoneStartGameMessageHandler::setup(
	Reference<EntityManager> entityManager_,
	Reference<GameLogWindowManager> gameLogWindowManager_,
	Reference<GameServerConnectionManager> gameServerConnectionManager_,
	CommandStack commandStack_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
	gameServerConnectionManager = gameServerConnectionManager_;
	commandStack = std::move(commandStack_);
}

void ZoneStartGameMessageHandler::set_player(Reference<Player> player_) {
	player = player_;
}

void ZoneStartGameMessageHandler::operator()(const Proto::ToClientMessage& message) {
	switch (message.message_case()) {
	case Proto::ToClientMessage::kStartGameResponse: // ログイン成功時の処理
	{
		const Proto::PayloadLobbyStartGameResponse& body = message.start_game_response();
		gameServerConnectionManager->on_connection_succeeded(); // 通知
		player->set_server_id(body.player_entity_id()); // サーバーIDの設定
		player->transform_mut().set_translate(Vector3{ body.position().x(), body.position().y(), body.position().z() }); // 初期位置の設定
		entityManager->register_server_id(body.player_entity_id(), player);
		szgInformation("Login succeeded. Id-\'{}\'", body.player_entity_id());
		gameLogWindowManager->add_log(
			GameLogWindowManager::Type::SystemMessage,
			L"Welcome to game!"
		);

		Proto::ToServerMessage enter;
		Proto::PayloadPlayerZoneEnterComplete* payload = enter.mutable_player_zone_enter_complete();
		NetworkCluster::SenderMut()->stack_packet(enter);
	}
	break;
	case Proto::ToClientMessage::kClientInitializerData:
	{
		const Proto::PayloadClientInitializerData& payload = message.client_initializer_data();
		szgInformation("Received initializer data. PlayerCount-\'{}\' EnemyCount-\'{}\'", payload.players_size(), payload.enemies_size());
		for (const auto& player : payload.players()) {
			const auto& entity_data = player.entity_data();

			commandStack(std::make_unique<ZoneLoginPlayerCommand>(
				entity_data.entity_id(),
				player.name(),
				Vector3{ entity_data.position().x(), entity_data.position().y(), entity_data.position().z() }
			));
		}
		for (const auto& enemy : payload.enemies()) {
			const auto& entity_data = enemy.entity_data();
			commandStack(std::make_unique<ZoneEnemySpawnCommand>(
				entity_data.entity_id(),
				enemy.enemy_type_id(),
				Vector3{ entity_data.position().x(), entity_data.position().y(), entity_data.position().z() }
			));
		}
	}
	default:
		break;
	}
}
