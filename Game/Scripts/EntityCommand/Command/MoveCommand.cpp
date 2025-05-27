#include "MoveCommand.h"

void MoveCommand::execute(Reference<IEntity> entity) {
	entity->move(xzDirection);
}
