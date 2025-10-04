#include "ZoneLoginPlayerCommand.h"

#include "Scripts/Manager/EntityManager.h"

#include "Scripts/Instance/Player/Player.h"

ZoneLoginPlayerCommand::ZoneLoginPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId_, const std::string& name_) :
	entityManager(entityManager_),
	serverId(serverId_),
	name(name_) {
}

void ZoneLoginPlayerCommand::execute() {
	Reference<Player> temp = entityManager->generate<Player>("Player.json");
	temp->set_name(name);
	temp->set_server_id(serverId);
	entityManager->register_server_id(serverId, temp);
}
