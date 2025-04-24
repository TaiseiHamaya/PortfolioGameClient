#pragma once

#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include <memory>

#include "Scripts/IEntity/BaseEntity.h"

class SphereCollider;

class Enemy : public BaseEntity {
public:
	Enemy(Vector3 translate);
	~Enemy() = default;

public:
	void initialize();
	void begin() override;
	void update() override;

public:
	void take_damage();

	bool is_destroy() const { return isDead; }

	void translate_force(const Vector3& translate_) { translate = translate_; };

private:
	Vector3 translate;
	Vector3 direction;

	float hitPoint{ 15 };
	bool isDead{ false };

public:
	inline static Reference<WorldInstance> player{};
};
