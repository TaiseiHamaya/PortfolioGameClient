#include "DeadEnemy.h"

#include <Engine/Runtime/Clock/WorldClock.h>

#include <Library/Math/Definition.h>

#include "Scripts/Util/RandomUtil.h"

DeadEnemy::DeadEnemy(const Vector3& position, const Quaternion& rotation, const Vector3& knockback_) :
	StaticMeshInstance("Enemy.gltf") {
	right = CVector3::RIGHT * rotation;
	translate = position;
	timer.set(0);
	isDestroy = false;

	knockback = knockback_ * 2.0f;
	knockback.y = 5.0f;
	shakeDirection = RandomOnSphere();
}

void DeadEnemy::update() {
	timer.ahead();
	if (timer.time() < 0.5f) {
		bool singbit = std::signbit(std::sin(timer.time() * PI * 20));
		if (singbit) {
			transform.set_translate(
				translate + shakeDirection * 0.3f
			);
		}
		else {
			transform.set_translate(
				translate - shakeDirection * 0.3f
			);
		}
	}
	else {
		knockback.x *= 0.95f;
		knockback.z *= 0.95f;
		knockback.y += -9.8f * WorldClock::DeltaSeconds();
		translate += knockback * WorldClock::DeltaSeconds();
		if (translate.y < 0) {
			isDestroy = true;
		}
		transform.set_translate(translate);
	}
}
