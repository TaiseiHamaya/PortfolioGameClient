#include "Idle.h"

#include "../IEntity.h"

void Idle::update() {
}

void Idle::reset() {
	timer.set(0);
	loopAnimation = true;
}

r32 Idle::progress() const {
	return 1.0f;
}

bool Idle::can_transition() const {
	return true;
}

bool Idle::end_action() const {
	return true;
}
