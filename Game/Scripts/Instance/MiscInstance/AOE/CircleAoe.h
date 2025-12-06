#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

/// <summary>
/// 円形サークル
/// </summary>
class CircleAoe : public szg::WorldInstance {
public:
	CircleAoe();
	~CircleAoe();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	/// <param name="radius"></param>
	/// <param name="duration_"></param>
	void initialize(const Vector3& position, float radius, float duration_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

public:
	bool is_end() const;

private:
	szg::WorldTimer timer;
	r32 duration{ 0.0f };

	Reference<szg::Rect3d> base;
	Reference<szg::Rect3d> effect;

	static constexpr r32 OFFSET_Y{ 0.02f };
	static constexpr r32 EFFECT_START_TIME{ 1.0f / 3.0f };
	static constexpr r32 AOE_ALPHA{ 0.5f };
	static constexpr r32 FADE_TIME{ 1.0f / 3.0f };
};
