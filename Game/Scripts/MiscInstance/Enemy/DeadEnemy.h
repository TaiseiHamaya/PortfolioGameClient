#pragma once

#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

class DeadEnemy final : public StaticMeshInstance {
public:
	DeadEnemy(const Vector3& position, const Quaternion& rotation, const Vector3& knockback_);
	~DeadEnemy() = default;

public:
	void update() override;

	bool is_destroy() const { return isDestroy; };

private:
	WorldTimer timer;
	bool isDestroy;
	Vector3 right;
	Vector3 shakeDirection;
	Vector3 translate;
	Vector3 knockback;
};

