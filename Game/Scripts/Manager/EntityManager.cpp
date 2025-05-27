#include "EntityManager.h"

void EntityManager::start(Reference<WorldManager> worldManager_, Reference<SkinningMeshDrawManager> skinDraw_, Reference<Rect3dDrawManager> rectDraw_) {
	worldManager = worldManager_;
	skinDraw = skinDraw_;
	rectDraw = rectDraw_;
}

void EntityManager::begin() {
	for (std::unique_ptr<IEntity>& entity : entities | std::views::values) {
		entity->begin();
	}
}

void EntityManager::update() {
	for (std::unique_ptr<IEntity>& entity : entities | std::views::values) {
		entity->update();
	}
}

void EntityManager::late_update() {
	for (std::unique_ptr<IEntity>& entity : entities | std::views::values) {
		entity->late_update();
	}
}

void EntityManager::destroy(u64 id) {
	if (entities.contains(id)) {
		entities.erase(id);
	}
}

Reference<IEntity> EntityManager::inquire(u64 id)  const {
	if (entities.contains(id)) {
		return entities.at(id);
	}
	return nullptr;
}
