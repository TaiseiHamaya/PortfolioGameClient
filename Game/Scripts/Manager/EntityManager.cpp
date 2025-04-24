#include "EntityManager.h"

void EntityManager::start(Reference<WorldManager> worldManager_) {
	worldManager = worldManager_;
}

void EntityManager::begin() {
	for (std::unique_ptr<BaseEntity>& entity : entities | std::views::values) {
		entity->begin();
	}
}

void EntityManager::update() {
	for (std::unique_ptr<BaseEntity>& entity : entities | std::views::values) {
		entity->update();
	}
}

void EntityManager::late_update() {
	for (std::unique_ptr<BaseEntity>& entity : entities | std::views::values) {
		entity->late_update();
	}
}

void EntityManager::destroy(u64 id) {
	if (entities.contains(id)) {
		entities.erase(id);
	}
}

Reference<BaseEntity> EntityManager::get_entity(u64 id) {
	if (entities.contains(id)) {
		return entities.at(id);
	}
	return nullptr;
}
