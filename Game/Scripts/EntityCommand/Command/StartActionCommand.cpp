#include "StartActionCommand.h"

StartActionCommand::StartActionCommand(std::string_view name) {
	actionName = name;
}

void StartActionCommand::execute(Reference<IEntity> entity) {
	entity->start_action(actionName);
}
