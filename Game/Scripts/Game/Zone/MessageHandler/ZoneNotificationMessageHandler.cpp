#include "ZoneNotificationMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneLoginPlayerCommand.h"
#include "Scripts/Game/Zone/Command/ZoneLogoutPlayerCommand.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"

void ZoneNotificationMessageHandler::setup(
	Reference<EntityManager> entityManager_,
	Reference<GameLogWindowManager> gameLogWindowManager_,
	CommandStack commandStack_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
	commandStack = std::move(commandStack_);
}

void ZoneNotificationMessageHandler::operator()(const Proto::ToClientMessage& message) {
	switch (message.message_case()) {
	case Proto::ToClientMessage::kZoneEnterNotification: // 他プレイヤーのログイン通知
	{
		const Proto::PayloadZoneEnterNotification& body = message.zone_enter_notification();
		szgInformation("Player added. Id-\'{}\' Name-\'{}\'", body.id(), body.username());
		gameLogWindowManager->add_log(
			GameLogWindowManager::Type::SystemMessage,
			ConvertString(std::format("Player {} has joined the game.", body.username()))
		);
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		commandStack(std::make_unique<ZoneLoginPlayerCommand>(body.id(), body.username(), position));
	}
	break;
	case Proto::ToClientMessage::kZoneExitNotification: // 他プレイヤーのログアウト通知
	{
		const Proto::PayloadZoneExitNotification& body = message.zone_exit_notification();
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		if (entity) {
			szgInformation("Player removed. Id-\'{}\' Name-\'{}\'", body.id(), entity->name_imm());
			gameLogWindowManager->add_log(
				GameLogWindowManager::Type::SystemMessage,
				ConvertString(std::format("Player {} has left the game.", entity->name_imm()))
			);
		}
		commandStack(std::make_unique<ZoneLogoutPlayerCommand>(body.id()));
	}
	break;
	default:
		break;
	}
}

