#include "ZoneEnemyMessageHandler.h"

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneEnemySpawnCommand.h"

void ZoneEnemyMessageHandler::setup(std::function<void(std::unique_ptr<IZoneCommand>)> commandStack_) {
	commandStack = std::move(commandStack_);
}

void ZoneEnemyMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	case Proto::ToClientMessage::kEnemySpawn: // 敵のスポーン
	{
		const Proto::PayloadEnemySpawn& body = packet.enemy_spawn();
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		commandStack(
			std::make_unique<ZoneEnemySpawnCommand>(
				body.entity_id(),
				body.enemy_type_id(),
				position
			)
		);
	}
	break;
	case Proto::ToClientMessage::kEnemyDespawn: // 敵のデスポーン
		break;
	default:
		break;
	}
}

