#include "ZoneLoginPlayerCommand.h"

#include "Scripts/Game/Zone/Zone.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Instance/Player/RemotePlayer.h"

ZoneLoginPlayerCommand::ZoneLoginPlayerCommand(u64 serverId_, const std::string& name_, const Vector3& position_) :
	serverId(serverId_),
	name(name_),
	position(position_) {
}

void ZoneLoginPlayerCommand::execute(Zone& zone) {
	Reference<RemotePlayer> temp = zone.entity_manager_mut()->generate<RemotePlayer>("Player.json");
	temp->set_name(name);
	temp->set_server_id(serverId);
	temp->transform_mut().set_translate(position);
	zone.entity_manager_mut()->register_server_id(serverId, temp);
}
