#pragma once

#include "./IZoneCommand.h"

#include <Library/Math/Vector3.h>

/// <summary>
/// 敵のスポーン
/// </summary>
class ZoneEnemySpawnCommand final: public IZoneCommand {
public:
	ZoneEnemySpawnCommand(u64 serverId_, u64 enemyTypeId_, Vector3 position_);

public:
	void execute(Zone& zone) override;

private:
	u64 serverId;
	u64 enemyTypeId;
	Vector3 position;
};
