#include "FollowCamera.h"

#include <algorithm>

#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Engine/Module/World/WorldManager.h>

#include <Library/Math/Definition.h>

#include "Scripts/Util/RandomUtil.h"

void FollowCamera::initialize() {
	Camera3D::initialize();

	lookAtInstance = world_manager()->create<WorldInstance>();
	reparent(lookAtInstance);
	destingRotation =
		Quaternion::EulerDegree(45, 0, 0);

	offset = { 0,0,-15 };
	padHandler.initialize({ PadID::Y });
	keyHandler.initialize({ KeyID::L });
}

void FollowCamera::update() {
	shakeTimer.back();
	if (shakeTimer.time() >= 0.0f) {
		bool singbit = std::signbit(std::sin(shakeTimer.time() * PI * 20));

		shakeOffset = shakeDirection * (1 - shakeTimer.time() / 0.2f);
		if (singbit) {
			shakeOffset *= -1;
		}
	}
	else {
		shakeOffset = CVector3::ZERO;
	}

	// 入力から回転に変換
	Vector2 rotateAngle = inputStickR * PI * WorldClock::DeltaSeconds();

	rotateAngle.y *= -1;
	// 平行成分と垂直成分でQuaternionを生成
	Quaternion holizontal = Quaternion::AngleAxis(CVector3::BASIS_Y, rotateAngle.x);
	Quaternion vertical = Quaternion::AngleAxis(CVector3::BASIS_X, rotateAngle.y);

	Vector3 beforeForward = CVector3::BASIS_Z * destingRotation;
	float forwardDot = Vector3::Dot(beforeForward, -CVector3::BASIS_Y);
	if (forwardDot >= 0.999f && rotateAngle.y > 0) {
		// 真下と視線のの差を出す。
		float angle = std::acos(std::clamp(forwardDot, -1.0f, 1.0f));
		if (angle >= 88.9f * ToRadian) {
			vertical = Quaternion::AngleAxis(CVector3::BASIS_X, angle - 1 * ToRadian);
		}
		else {
			vertical = CQuaternion::IDENTITY;
		}
	}
	else if (forwardDot <= 5.1f * ToRadian && rotateAngle.y < 0) {
		Vector3 beforeUpward = CVector3::BASIS_Y * destingRotation;
		float upwardDot = Vector3::Dot(beforeUpward, CVector3::BASIS_Y);
		float angle = std::acos(std::clamp(upwardDot, -1.0f, 1.0f));
		// 水平より5度下を向かせる
		if (angle >= 5.01f * ToRadian) {
			vertical = Quaternion::AngleAxis(CVector3::BASIS_X, -(angle - 5 * ToRadian));
		}
		// 変化する角度があまりに小さい場合や範囲を超える場合は回転させない
		else {
			vertical = CQuaternion::IDENTITY;
		}
	}
	// 垂直->元->平行で適用させる
	destingRotation = holizontal * destingRotation * vertical;

	if (isPressX) {
		Vector3 targetForward = CVector3::BASIS_Z * target->get_transform().get_quaternion();
		destingRotation = Quaternion::LookForward(targetForward) * Quaternion::AngleAxis(CVector3::BASIS_X, PI / 8);
	}

	transform.set_quaternion(
		Quaternion::Slerp(transform.get_quaternion(), destingRotation, 0.1f)
	);

	// ターゲットが設定されていない場合は視点移動しない
	if (!target) {
		return;
	}
	// 今のworld座標と注視対象のworld座標で補完
	Vector3 lookAt = Vector3::Lerp(lookAtInstance->world_position(), target->world_position(), 0.4f);
	lookAtInstance->get_transform().set_translate(lookAt);
	// offsetを回転させて視線を向ける
	Vector3 translate = offset * transform.get_quaternion();
	transform.set_translate(translate + shakeOffset);
}

void FollowCamera::input() {
	inputStickR = Input::StickR();
	if (inputStickR.length() == 0) {
		inputStickR = InputAdvanced::PressArrow();
	}
	isPressX = padHandler.trigger(PadID::Y) || keyHandler.trigger(KeyID::L);
}

void FollowCamera::do_shake() {
	shakeTimer.set(0.2f);
	Vector3 base = CVector3::RIGHT * Quaternion::AngleAxis(CVector3::FORWARD, RandomEngine::Random01MOD() * PI2);
	base = base * world_affine().get_basis();
	shakeDirection = base * 0.4f;
}

void FollowCamera::set_offset(const Vector3& offset_) {
	offset = offset_;
}

const Vector3& FollowCamera::get_offset() const {
	return offset;
}

void FollowCamera::set_target(Reference<const WorldInstance> target_) {
	target = target_;
}

#ifdef _DEBUG

#include <imgui.h>

void FollowCamera::debug_gui() {
	ImGui::Begin("Camera3D");
	ImGui::DragFloat3("Offset", &offset.x, 0.1f);
	Camera3D::debug_gui();
	ImGui::End();
}
#endif // _DEBUG
