#pragma once

#include <memory>
#include <vector>

#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Module/World/WorldInstance/WorldInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/IEntity/IEntity.h"
#include "Scripts/Util/LookAtRect.h"

class PaladinHolySpiritEffectTarget final : public WorldInstance {
public:
	PaladinHolySpiritEffectTarget() = default;
	~PaladinHolySpiritEffectTarget() = default;

public:
	void initialize();
	void setup(Reference<IEntity> self, Reference<IEntity> target);
	void update() override;

private:
	WorldTimer timer;

	Reference<WorldInstance> camera;

	std::unique_ptr<LookAtRect> absorption;
	std::unique_ptr<WorldInstance> centerConstraint;
	std::vector<std::unique_ptr<LookAtRect>> centerBillboards;
	std::unique_ptr<LookAtRect> lightBillboard;
	std::unique_ptr<ParticleEmitterInstance> etherDustEmitter;
	std::unique_ptr<ParticleEmitterInstance> shiningEmitter;
};
