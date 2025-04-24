#pragma once

#include <list>

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>

class Enemy;
class CollisionManager;
class GameCallback;
class WorldManager;
class DeadEnemy;
class StaticMeshDrawManager;
class SkinningMeshDrawManager;

class EnemyManager {
public:
	EnemyManager(Reference<WorldManager> worldManager_);
	~EnemyManager();

public:
	void begin();

	void update();
	void late_update();

public:
	void add_enemy(Vector3 position);
	std::list<std::unique_ptr<Enemy>>::iterator erase_enemy(const std::list<std::unique_ptr<Enemy>>::iterator& itr);

private:
	std::list<std::unique_ptr<Enemy>> enemies;

	Reference<WorldManager> worldManager;

public:
	inline static Reference<StaticMeshDrawManager> staticMeshDrawManager{ nullptr };
	inline static Reference<SkinningMeshDrawManager> skinningMeshDrawManager{ nullptr };
};
