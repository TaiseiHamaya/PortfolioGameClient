#include "ZoneStartGameMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"

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
	}
	break;
	default:
		break;
	}
}
