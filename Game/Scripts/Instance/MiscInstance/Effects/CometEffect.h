#pragma once

#include <Engine/Module/Render/RenderPipeline/Posteffect/RadialBlur/RadialBlurPipeline.h>
#include <Engine/Module/World/Camera/Camera3D.h>
#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Instance/MiscInstance/Effects/IEffectInstance.h"

/// <summary>
/// コメットのエフェクト
/// </summary>
class CometEffect final : public IEffectInstance {
public:
	CometEffect();
	~CometEffect();

public:
	void initialize(const Vector3& position);

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
	szg::WorldTimer timer;

	Reference<szg::ParticleEmitterInstance> dustCloudParticle0;
	Reference<szg::ParticleEmitterInstance> dustCloudParticle1;
	//std::vector<LookAtRect> dustClouds;
	Reference<szg::StaticMeshInstance> cometBody;
	Reference<szg::Rect3d> cometFire;
	Reference<szg::Rect3d> groundEffect;

	Reference<szg::RadialBlurPipeline::Data> blurData;

	szg::JsonAsset json;
	Color3 CometBodyColor{};
	Vector2 CometFireSize{};
	Vector2 CometFirePivot{};
	Color4 CometFireColor{};
	Vector2 GroundEffectSize{};
	Vector2 GroundEffectPivot{};
	Color4 GroundEffectColor{};
	r32 CometHight{};
	r32 FallTime{};
	r32 GroundEffectTime{};
	r32 BlurLengthMax{};
	r32 BlurWeight{};

public:
	inline static Reference<szg::Camera3D> camera{ nullptr };
};
