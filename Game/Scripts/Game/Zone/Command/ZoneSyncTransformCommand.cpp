#include "ZoneSyncTransformCommand.h"

#include "Scripts/Instance/IEntity/IEntity.h"

ZoneSyncTransformCommand::ZoneSyncTransformCommand(Reference<IEntity> entity_, const std::chrono::system_clock::time_point& time_, const Vector3& position_) :
	entity(entity_),
	time(time_),
	position(position_) {
}

void ZoneSyncTransformCommand::execute(Zone&) {
	if (entity) {
		entity->move_to(time, position);
	}
}
