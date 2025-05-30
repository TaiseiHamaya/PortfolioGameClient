#include "PlayerActionCommand .h"

PlayerActionCommand::PlayerActionCommand(std::unique_ptr<ISkillAction> skillAction_) {
	skillAction = std::move(skillAction_);
}

void PlayerActionCommand::execute(Reference<IEntity> entity) {
}
