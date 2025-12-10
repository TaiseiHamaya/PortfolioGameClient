#pragma once

#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Module/World/Camera/Camera3D.h>
#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Runtime/Input/InputHandler.h>

/// <summary>
/// 追跡カメラ
/// </summary>
class FollowCamera final : public szg::Camera3D {
public:
	void initialize() override;
	void update();

	void input();

public:
	/// <summary>
	/// 画面揺れの実行
	/// </summary>
	void do_shake();

public:
	void set_offset(const Vector3& offset_);
	const Vector3& get_offset() const;

	void set_target(Reference<const szg::WorldInstance> target_);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif // DEBUG_FEATURES_ENABLE

private:
	Vector3 offset;

	bool isPressX;
	szg::InputHandler<szg::KeyID> keyHandler;
	szg::InputHandler<szg::PadID> padHandler;
	Vector2 inputStickR;

	Reference<const WorldInstance> target;
	Reference<WorldInstance> lookAtInstance;
	Quaternion destingRotation;

	Vector3 shakeOffset;
	Vector3 shakeDirection;
	szg::WorldTimer shakeTimer;

	r32 SlerpStrength;
	r32 FollowStrength;
	r32 MaxAngleDownward;
	r32 MaxAngleHorizontal;
	r32 ShakeTime;
	r32 ShakePower;

#ifdef DEBUG_FEATURES_ENABLE
	szg::JsonAsset json;
#endif // DEBUG_FEATURES_ENABLE
};

