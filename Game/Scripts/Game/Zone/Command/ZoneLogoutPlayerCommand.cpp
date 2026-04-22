#include "ZoneLogoutPlayerCommand.h"

#include "Scripts/Game/Zone/Zone.h"
#include "Scripts/Manager/EntityManager.h"

ZoneLogoutPlayerCommand::ZoneLogoutPlayerCommand(u64 serverId_) :
	serverId(serverId_) {
}

void ZoneLogoutPlayerCommand::execute(Zone& zone) {
	zone.entity_manager_mut()->destroy(serverId);
}
