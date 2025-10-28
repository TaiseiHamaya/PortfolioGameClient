#include "ZoneLoginPlayerCommand.h"

#include "Scripts/Manager/EntityManager.h"

#include "Scripts/Instance/Player/RemotePlayer.h"

ZoneLoginPlayerCommand::ZoneLoginPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId_, const std::string& name_, const Vector3& position_) :
	entityManager(entityManager_),
	serverId(serverId_),
	name(name_),
	position(position_) {
}

void ZoneLoginPlayerCommand::execute() {
	Reference<RemotePlayer> temp = entityManager->generate<RemotePlayer>("Player.json");
	temp->set_name(name);
	temp->set_server_id(serverId);
	temp->get_transform().set_translate(position);
	entityManager->register_server_id(serverId, temp);
}
