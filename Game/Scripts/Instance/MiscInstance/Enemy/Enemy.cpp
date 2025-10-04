#include "Enemy.h"

#include <Library/Utility/Tools/SmartPointer.h>
#include <Engine/Module/World/Collision/Collider/SphereCollider.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Library/Utility/Tools/RandomEngine.h>
#include <Engine/Module/World/WorldManager.h>

#include "Scripts/Extension/Util/RandomUtil.h"

void Enemy::move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) {
	// do nothing
}
