#pragma once

#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Extension/Util/LookAtRect.h"

class HitAnimation final {
public:
	HitAnimation(Vector3&& position);
	~HitAnimation() = default;

	__CLASS_NON_COPYABLE(HitAnimation)

public:
	void begin();
	void update();
	
public:
	bool is_end() const;

private:
	Reference<LookAtRect> hitBillboard;
	Reference<ParticleEmitterInstance> emitter;

	WorldTimer hitAnimationTimer;

public:
	static inline Reference<WorldRoot> worldRoot{};
};

