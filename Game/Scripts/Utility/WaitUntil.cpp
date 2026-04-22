#include "WaitUntil.h"

#include <Library/Math/Definition.h>

void WaitUntil::initialize(Reference<szg::Rect3d> rect_) {
	rect = rect_;
	rect->set_active(false);
}

void WaitUntil::reset(std::function<bool()> condition_) {
	condition = condition_;
	rect->set_active(true);
	timer.set(0);
}

bool WaitUntil::update() {
	if (!condition) {
		return true;
	}
	timer.ahead();

	if (condition()) {
		condition = nullptr;
		rect->set_active(false);
		return true;
	}
	else {
		rect->transform_mut().set_quaternion(
			Quaternion::LookForward(CVector3::BACKWARD) * 
			Quaternion::AngleAxis(CVector3::FORWARD, i32(timer * 8) % 8 * PI2 / 8)
		);
		return false;
	}
}
