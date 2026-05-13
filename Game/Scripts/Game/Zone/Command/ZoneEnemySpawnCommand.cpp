#include "ZoneEnemySpawnCommand.h"

#include "Scripts/Game/Zone/Zone.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"

ZoneEnemySpawnCommand::ZoneEnemySpawnCommand(u64 serverId_, u64 enemyTypeId_, Vector3 position_) :
	serverId(serverId_),
	enemyTypeId(enemyTypeId_),
	position(position_) {
}

void ZoneEnemySpawnCommand::execute(Zone& zone) {
	zone.enemy_manager_mut()->generate("RedComet.json", position, serverId);
	Reference<IEntity> enemy = zone.entity_manager_imm()->inquire_server_id(serverId);
	if (enemy) {
		szgInformation("Enemy Spawned. Name-\'{}({})\' ServerID-\'{}\'", enemy->name_imm(), enemyTypeId, serverId);
	}

	zone.player_mut()->set_target(enemy);
}
