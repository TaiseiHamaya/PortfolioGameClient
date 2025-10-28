#include "ZoneLogoutPlayerCommand.h"

#include "Scripts/Manager/EntityManager.h"

ZoneLogoutPlayerCommand::ZoneLogoutPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId_) :
	entityManager(entityManager_), serverId(serverId_) {
}

void ZoneLogoutPlayerCommand::execute() {
	entityManager->destroy(serverId);
}
