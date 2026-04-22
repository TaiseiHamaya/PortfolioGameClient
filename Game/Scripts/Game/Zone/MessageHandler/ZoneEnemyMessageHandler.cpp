#include "ZoneEnemyMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"

void ZoneEnemyMessageHandler::setup(Reference<EnemyManager> enemyManager_, Reference<EntityManager> entityManager_) {
	enemyManager = enemyManager_;
	entityManager = entityManager_;
}

void ZoneEnemyMessageHandler::set_player(Reference<Player> player_) {
	player = player_;
}

void ZoneEnemyMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	case Proto::ToClientMessage::kEnemySpawn: // 敵のスポーン
	{
		const Proto::PayloadEnemySpawn& body = packet.enemy_spawn();
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		enemyManager->generate(std::format("{}.json", body.name()), position, body.id());
		Reference<IEntity> enemy = entityManager->inquire_server_id(body.id());
		if (enemy) {
			szgInformation("Enemy Spawned. Id-\'{}\' Name-\'{}\'", body.id(), enemy->name_imm());
		}
		player->set_target(enemy);
	}
	break;
	case Proto::ToClientMessage::kEnemyDespawn: // 敵のデスポーン
		break;
	default:
		break;
	}
}

