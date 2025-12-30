#include "JumpAction.h"

#include "../IEntity.h"

void JumpAction::start() {
}

void JumpAction::update() {
	timer.ahead();
}

void JumpAction::reset() {
}

bool JumpAction::can_transition() const {
	return true;
}

bool JumpAction::end_action() const {
	return owner->transform_imm().get_translate().y <= 0;
}
