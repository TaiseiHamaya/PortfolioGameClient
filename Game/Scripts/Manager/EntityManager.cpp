#include "EntityManager.h"

void EntityManager::prev_update() {
}

void EntityManager::post_update() {
	for (u64 serverId : removedEntityIds) {
		Reference<IEntity> entity = inquire_server_id(serverId);
		if (entity) {
			worldRoot->destroy(entity);
			entities.erase(entity->local_id());
			entityRefByServerId.erase(serverId);
		}
	}
}

void EntityManager::setup(Reference<WorldRoot> worldRoot_) {
	worldRoot = worldRoot_;
}

void EntityManager::destroy(u64 serverId) {
	removedEntityIds.emplace(serverId);
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
