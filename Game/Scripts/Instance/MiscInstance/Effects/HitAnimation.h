#pragma once

#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Particle/ParticleEmitterInstance.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Extension/Util/LookAtRect.h"

/// <summary>
/// ヒットアニメーション
/// ヒット時の視覚的フィードバック（ビルボード表示とパーティクル発生）をプレイ
/// </summary>
class HitAnimation final {
public:
	HitAnimation(Vector3&& position);
	~HitAnimation() = default;

	SZG_CLASS_MOVE_ONLY(HitAnimation)

public:
	void begin();
	void update();
	
public:
	bool is_end() const;

private:
	Reference<LookAtRect> hitBillboard;
	Reference<szg::ParticleEmitterInstance> emitter;

	szg::WorldTimer hitAnimationTimer;

public:
	static inline Reference<szg::WorldRoot> worldRoot{};
};

