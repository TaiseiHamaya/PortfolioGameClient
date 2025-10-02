#include "IdleAction.h"

IdleAction::IdleAction() : IActionBasic() {
	//useAnimationName = 
}

void IdleAction::start() {
	timer.ahead();
}

void IdleAction::update() {
}

void IdleAction::reset() {
	timer.set(0);
	loopAnimation = true;
}

r32 IdleAction::progress() const {
	return 1.0f;
}

bool IdleAction::can_transition() const {
	return true;
}

bool IdleAction::end_action() const {
	return false;
}
