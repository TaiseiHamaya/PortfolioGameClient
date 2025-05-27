#include "JumpCommand.h"

void JumpCommand::execute(Reference<IEntity> entity) {
	if (entity) {
		entity->jump();
	}
}
