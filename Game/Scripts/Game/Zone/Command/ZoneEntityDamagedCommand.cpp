#include "ZoneEntityDamagedCommand.h"

#include "Scripts/Instance/IEntity/IEntity.h"

ZoneEntityDamagedCommand::ZoneEntityDamagedCommand(Reference<IEntity> entity_, const u32 damage_) {
	entity = entity_;
	damage = damage_;
}

void ZoneEntityDamagedCommand::execute() {
	if (!entity) {
		return;
	}
	entity->on_damaged(damage);
}
