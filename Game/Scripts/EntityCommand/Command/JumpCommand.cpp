#include "JumpCommand.h"

void JumpCommand::execute(Reference<BaseEntity> entity) {
	if (entity) {
		entity->jump();
	}
}
