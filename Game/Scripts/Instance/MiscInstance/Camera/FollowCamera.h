#pragma once

#include <Engine/Module/World/Camera/Camera3D.h>
#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Runtime/Input/InputHandler.h>

/// <summary>
/// 追跡カメラ
/// </summary>
class FollowCamera final : public Camera3D {
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

	void set_target(Reference<const WorldInstance> target_);

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	Vector3 offset;

	bool isPressX;
	InputHandler<KeyID> keyHandler;
	InputHandler<PadID> padHandler;
	Vector2 inputStickR;

	Reference<const WorldInstance> target;
	std::unique_ptr<WorldInstance> lookAtInstance;
	Quaternion destingRotation;

	Vector3 shakeOffset;
	Vector3 shakeDirection;
	WorldTimer shakeTimer;
};

