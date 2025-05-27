#include "StartActionCommand.h"

void StartActionCommand::execute(Reference<IEntity> entity) {
	entity->start_action(actionIndex);
}
