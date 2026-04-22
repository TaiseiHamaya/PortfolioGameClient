#include "ZoneLogoutMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneLogoutPlayerCommand.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"

void ZoneLogoutMessageHandler::setup(
	Reference<EntityManager> entityManager_,
	Reference<GameLogWindowManager> gameLogWindowManager_,
	CommandStack commandStack_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
	commandStack = std::move(commandStack_);
}

void ZoneLogoutMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	case Proto::ToClientMessage::kLogoutResponse: // ログアウト結果
		break;
	case Proto::ToClientMessage::kZoneExitNotification: // 他プレイヤーのログアウト通知
	{
		const Proto::PayloadZoneExitNotification& body = packet.zone_exit_notification();
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

