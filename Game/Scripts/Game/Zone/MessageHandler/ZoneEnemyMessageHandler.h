#pragma once

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class EntityManager;
class EnemyManager;
class Player;

/// <summary>
/// 敵メッセージのハンドラ
/// </summary>
class ZoneEnemyMessageHandler {
public:
	ZoneEnemyMessageHandler() = default;
	~ZoneEnemyMessageHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneEnemyMessageHandler)

public:
	void setup(
		Reference<EnemyManager> enemyManager_,
		Reference<EntityManager> entityManager_
	);

	void set_player(Reference<Player> player_);

	void operator()(const Proto::ToClientMessage& packet);

private:
	Reference<EnemyManager> enemyManager;
	Reference<EntityManager> entityManager;
	Reference<Player> player;
};
