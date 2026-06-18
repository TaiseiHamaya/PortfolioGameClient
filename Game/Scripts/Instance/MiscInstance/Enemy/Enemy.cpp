#include "Enemy.h"

#include <Library/Utility/Tools/SmartPointer.h>
#include <Engine/Module/World/Collider/SphereCollider.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Library/Utility/Tools/RandomEngine.h>
#include <Engine/Runtime/Scene/World/WorldRoot.h>

#include "Scripts/Extension/Util/RandomUtil.h"

void Enemy::move_to(const std::chrono::system_clock::time_point&, const Vector3&) {
	// do nothing
}
