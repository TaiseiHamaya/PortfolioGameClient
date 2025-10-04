#include "TriggerOnce.h"

TriggerOnce::TriggerOnce(std::function<bool(void)> trigger_, std::function<void(void)> callFunction_) {
	trigger = trigger_;
	callFunction = callFunction_;
}

void TriggerOnce::update() {
	if (!called) {
		return;
	}

	if (!trigger || !callFunction) {
		return;
	}

	if (trigger()) {
		callFunction();
		called = true;
	}
}

void TriggerOnce::reset() {
	called = false;
}

void TriggerOnce::reset_trigger(std::function<bool(void)> trigger_) {
	trigger = trigger_;
}

void TriggerOnce::set_call(std::function<bool(void)> callFunction_) {
	callFunction = callFunction_;
}
