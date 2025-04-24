#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Util/LookAtRect.h"

class StaticMeshDrawManager;
class Rect3dDrawManager;

class CometEffect final : public WorldInstance {
public:
	CometEffect();
	~CometEffect();

public:
	void initialize(const Vector3& position);
	void start(Reference<StaticMeshDrawManager> meshDraw, Reference<Rect3dDrawManager> rectDraw);

public:
	void update();

	void draw_particle();

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

public:
	inline static Reference<WorldInstance> camera{ nullptr };
};
