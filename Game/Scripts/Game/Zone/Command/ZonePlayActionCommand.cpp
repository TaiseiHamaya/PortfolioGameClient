#include "ZonePlayActionCommand.h"

#include "Scripts/Instance/IEntity/IEntity.h"

ZonePlayActionCommand::ZonePlayActionCommand(Reference<IEntity> entity_, const u32 actionId_, Reference<IEntity> targetEntity_) {
	entity = entity_;
	actionId = actionId_;
	targetEntity = targetEntity_;
}

void ZonePlayActionCommand::execute() {
	if(!entity || !targetEntity) {
		return;
	}
	entity->set_target(targetEntity);
	entity->start_action("PaladinHolySpirit"_sh);
}
