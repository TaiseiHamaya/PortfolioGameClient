#pragma once

#include <memory>
#include <vector>

#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/MiscInstance/Effects/IEffectInstance.h"
#include "Scripts/IEntity/IEntity.h"
#include "Scripts/Util/LookAtRect.h"

class PaladinHolySpiritEffectTarget final : public IEffectInstance {
public:
	PaladinHolySpiritEffectTarget();
	~PaladinHolySpiritEffectTarget() = default;

public:
	void initialize(const Vector3& position);
	void setup(Reference<StaticMeshDrawManager> meshDrawManager, Reference<Rect3dDrawManager> rectDrawManager) override;
	void terminate(Reference<StaticMeshDrawManager> meshDrawManager, Reference<Rect3dDrawManager> rectDrawManager) override;
	void update() override;
	void draw_particle() const override;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif //DEBUG_FEATURES_ENABLE

private:
	WorldTimer timer;

	Reference<WorldInstance> camera;

	std::unique_ptr<LookAtRect> absorption;
	std::unique_ptr<WorldInstance> centerConstraint;
	std::vector<std::unique_ptr<Rect3d>> centerBillboards;
	std::unique_ptr<LookAtRect> lightBillboard;
	std::unique_ptr<ParticleEmitterInstance> etherDustEmitter;
	std::unique_ptr<ParticleEmitterInstance> shiningEmitter;
};
