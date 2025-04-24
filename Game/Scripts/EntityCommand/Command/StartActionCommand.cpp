#include "StartActionCommand.h"

void StartActionCommand::execute(Reference<BaseEntity> entity) {
	entity->start_action(actionIndex);
}
