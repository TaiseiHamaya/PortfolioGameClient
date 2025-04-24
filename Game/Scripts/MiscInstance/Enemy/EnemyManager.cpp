#include "EnemyManager.h"

#include "Enemy.h"
#include "DeadEnemy.h"

#include <Engine/Module/World/Collision/Collider/SphereCollider.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

EnemyManager::EnemyManager(Reference<WorldManager> worldManager_) :
	worldManager(worldManager_) {
	add_enemy(Vector3{ 0,0,3 });
	add_enemy(Vector3{ 4,0,3 });
	add_enemy(Vector3{ -4,0,3 });
	add_enemy(Vector3{ 0,0,6 });
	add_enemy(Vector3{ 4,0,6 });
	add_enemy(Vector3{ -4,0,6 });
}

EnemyManager::~EnemyManager() = default;

void EnemyManager::begin() {
	for (std::unique_ptr<Enemy>& enemy : enemies) {
		enemy->begin();
	}
}

void EnemyManager::update() {
	for (std::unique_ptr<Enemy>& enemy : enemies) {
		enemy->update();
	}

	auto removed = enemies.remove_if([&](const std::unique_ptr<Enemy>& elem) {
		if (elem->is_destroy()) {
			skinningMeshDrawManager->unregister_instance(elem);
			return true;
		}
		return false;
	});
}

void EnemyManager::late_update() {
	for (std::unique_ptr<Enemy>& enemy : enemies) {
		enemy->late_update();
	}
}

void EnemyManager::add_enemy(Vector3 position) {
	std::unique_ptr<Enemy>& enemy = enemies.emplace_back(worldManager->create<Enemy>(nullptr, position));
	enemy->initialize();
}

std::list<std::unique_ptr<Enemy>>::iterator EnemyManager::erase_enemy(const std::list<std::unique_ptr<Enemy>>::iterator& itr) {
	return enemies.erase(itr);
}
