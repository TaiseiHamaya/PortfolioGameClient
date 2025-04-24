#include "MoveCommand.h"

void MoveCommand::execute(Reference<BaseEntity> entity) {
	entity->move(xzDirection);
}
