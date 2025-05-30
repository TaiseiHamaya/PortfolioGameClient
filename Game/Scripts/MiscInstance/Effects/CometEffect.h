#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Module/World/Camera/Camera3D.h>

#include "Scripts/MiscInstance/Effects/IEffectInstance.h"

class StaticMeshDrawManager;
class Rect3dDrawManager;
struct BlurInfo;

class CometEffect final : public IEffectInstance {
public:
	CometEffect();
	~CometEffect();

public:
	void initialize(const Vector3& position, Reference<BlurInfo> blur_);
	void setup(Reference<StaticMeshDrawManager> meshDraw, Reference<Rect3dDrawManager> rectDraw) override;
	void terminate(Reference<StaticMeshDrawManager> meshDraw, Reference<Rect3dDrawManager> rectDraw) override;

public:
	void update() override;

	void draw_particle() const override;

public:
	bool is_end() const;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	WorldTimer timer;

	std::unique_ptr<ParticleEmitterInstance> dustCloudParticle0;
	std::unique_ptr<ParticleEmitterInstance> dustCloudParticle1;
	//std::vector<LookAtRect> dustClouds;
	std::unique_ptr<StaticMeshInstance> cometBody;
	std::unique_ptr<Rect3d> cometFire;
	std::unique_ptr<Rect3d> groundEffect;

	Reference<BlurInfo> blurData;

public:
	inline static Reference<Camera3D> camera{ nullptr };
};
