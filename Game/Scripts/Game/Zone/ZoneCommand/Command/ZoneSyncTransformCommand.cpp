#include "ZoneSyncTransformCommand.h"

#include "Scripts/Instance/IEntity/IEntity.h"

ZoneSyncTransformCommand::ZoneSyncTransformCommand(Reference<IEntity> entity_, const std::chrono::steady_clock::time_point& time, const Vector3& position_) :
	entity(entity_),
	position(position_) {
}

void ZoneSyncTransformCommand::execute() {
	if (entity) {
		entity->move_to(time, position);
	}
}
