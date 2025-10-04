#include "EntityManager.h"

void EntityManager::setup(Reference<WorldManager> worldManager_, Reference<SkinningMeshDrawManager> skinDraw_, Reference<Rect3dDrawManager> rectDraw_) {
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

void EntityManager::destroy(u64 serverId) {
	Reference<IEntity> entity = inquire_server_id(serverId);
	if (entity) {
		entities.erase(entity->local_id());
		entityRefByServerId.erase(serverId);
		worldManager->erase(entity);
	}
}

Reference<IEntity> EntityManager::inquire_server_id(u64 id) const {
	if (entityRefByServerId.contains(id)) {
		return entityRefByServerId.at(id);
	}
	return nullptr;
}

Reference<IEntity> EntityManager::inquire_local_id(u64 id) const {
	if (entities.contains(id)) {
		return entities.at(id);
	}
	return nullptr;
}

void EntityManager::register_server_id(u64 serverId, Reference<IEntity> entity) {
	entityRefByServerId.emplace(serverId, entity);
}
