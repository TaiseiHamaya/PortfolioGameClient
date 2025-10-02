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
	std::unique_ptr<LookAtRect> hitBillboard;
	std::unique_ptr<ParticleEmitterInstance> emitter;

	WorldTimer hitAnimationTimer;

public:
	static inline Reference<WorldManager> worldManager{};
};

