#include "ZoneLoginMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneLoginPlayerCommand.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Network/GameServer/GameServerConnectionManager.h"

void ZoneLoginMessageHandler::setup(
	Reference<EntityManager> entityManager_,
	Reference<GameLogWindowManager> gameLogWindowManager_,
	CommandStack commandStack_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
	commandStack = std::move(commandStack_);
}

void ZoneLoginMessageHandler::set_player(Reference<Player> player_) {
	player = player_;
}

void ZoneLoginMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	//case Proto::ToClientMessage::kLoginResult: // ログイン成功時の処理
	//{
	//	const Proto::PayloadLoginResult& body = packet.login_result();
	//	gameServerConnectionManager->on_connection_succeeded(); // 通知
	//	player->set_server_id(body.id()); // サーバーIDの設定
	//	player->transform_mut().set_translate(Vector3{ body.position().x(), body.position().y(), body.position().z() }); // 初期位置の設定
	//	entityManager->register_server_id(body.id(), player);
	//	szgInformation("Login succeeded. Id-\'{}\'", body.id());
	//	gameLogWindowManager->add_log(
	//		GameLogWindowManager::Type::SystemMessage,
	//		L"Welcome to game!"
	//	);
	//}
	//break;
	case Proto::ToClientMessage::kZoneEnterNotification: // 他プレイヤーのログイン通知
	{
		const Proto::PayloadZoneEnterNotification& body = packet.zone_enter_notification();
		szgInformation("Player added. Id-\'{}\' Name-\'{}\'", body.id(), body.username());
		gameLogWindowManager->add_log(
			GameLogWindowManager::Type::SystemMessage,
			ConvertString(std::format("Player {} has joined the game.", body.username()))
		);
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		commandStack(std::make_unique<ZoneLoginPlayerCommand>(body.id(), body.username(), position));
	}
	break;
	default:
		break;
	}
}
