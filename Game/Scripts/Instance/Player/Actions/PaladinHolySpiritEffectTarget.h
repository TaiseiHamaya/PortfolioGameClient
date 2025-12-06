#pragma once

#include <memory>
#include <vector>

#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Extension/Util/LookAtRect.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/MiscInstance/Effects/IEffectInstance.h"

class PaladinHolySpiritEffectTarget final : public IEffectInstance {
public:
	struct RectEffectValues {
		Vector2 size;
		Vector2 pivot;
		Color4 color;
		r32 beginTime;
		r32 endTime;
		Vector3 beginScale;
	};

public:
	PaladinHolySpiritEffectTarget();
	~PaladinHolySpiritEffectTarget() = default;

public:
	void initialize(const Vector3& position);
	
	void update() override;
	
	void draw_particle() const override;

public:
	void load_constant_values();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif //DEBUG_FEATURES_ENABLE

private:
	szg::WorldTimer timer;

	Reference<szg::WorldInstance> camera;

	Reference<LookAtRect> absorption;
	Reference<szg::WorldInstance> centerConstraint;
	std::vector<Reference<szg::Rect3d>> centerBillboards;
	Reference<LookAtRect> lightBillboard;
	Reference<szg::ParticleEmitterInstance> etherDustEmitter;
	Reference<szg::ParticleEmitterInstance> shiningEmitter;

	szg::JsonAsset json;
	RectEffectValues absorptionEffectValues{};
	RectEffectValues centerEffectValues{};
	RectEffectValues lightEffectValues{};
};
