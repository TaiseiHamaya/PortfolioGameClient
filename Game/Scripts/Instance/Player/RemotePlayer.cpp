#include "RemotePlayer.h"

#include <Library/Utility/Tools/Functions.h>

#include "../IEntity/Actions/JumpAction.h"
#include "Actions/PaladinHolySpirit.h"

void RemotePlayer::initialize(const std::filesystem::path& file) {
	IEntity::initialize(file);
	// プレイヤーのアクションを登録
	auto jumpAction = std::make_unique<JumpAction>();
	jumpAction->setup(this, std::format("{}.gltf-{}", file.stem().string(), "Dash"));
	actionList.emplace("Jump", std::move(jumpAction));
	auto paladinHolySpirit = std::make_unique<PaladinHolySpirit>();
	paladinHolySpirit->setup(this, std::format("{}.gltf-{}", file.stem().string(), "AttackSky"));
	actionList.emplace("PaladinHolySpirit", std::move(paladinHolySpirit));
}

void RemotePlayer::update() {
	// 移動補完
	calculate_position();

	IEntity::update();
}

void RemotePlayer::move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) {
	waypoints.emplace_back(time, position);
}

void RemotePlayer::calculate_position() {
	if (waypoints.size() < 2) {
		if (waypoints.size() == 1) {
			const auto& [_, firstPosition] = waypoints.front();
			transform.set_translate(firstPosition);
		}
		return;
	}

	auto now = std::chrono::steady_clock::now();
	const auto& [firstTime, firstPosition] = waypoints.front();
	const auto& [secondTime, secondPosition] = *(std::next(waypoints.begin()));

	if (firstTime == secondTime) {
		// これはないと信じたい
		transform.set_translate(secondPosition);
		waypoints.pop_front();
		return;
	}
	if (now < firstTime) {
		// たぶんめっちゃ遅延したとき
		transform.set_translate(firstPosition);
		return;
	}
	// 時間を逆補完
	auto diffDuration = secondTime - firstTime;
	auto elapsedDuration = now - firstTime;
	r32 param = static_cast<r32>(elapsedDuration.count()) / static_cast<r32>(diffDuration.count());
	param = eps::saturate(param);
	
	// 位置補完
	Vector3 position = eps::lerp(firstPosition, secondPosition, param);
	// 今の位置を記録
	Vector3 dest = transform.get_translate();
	transform.set_translate(position);

	// 回転
	Vector3 diff = position - dest;
	Vector3 direction = diff.normalize_safe();
	// xz方向の向いている方向
	Vector3 xzDirection = Vector3{ direction.x, 0.0f,direction.z };
	if (xzDirection.length() <= 0.01f) {
		// 向く方向
		Quaternion forwardTo{ Quaternion::LookForward(xzDirection.normalize()) };
		// Slerp補完
		transform.set_quaternion(
			Quaternion::Slerp(transform.get_quaternion(), forwardTo, 0.1f)
		);
	}
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
void RemotePlayer::debug_gui() {
	ImGui::Begin("Player");
	for (auto& action : actionList | std::views::values) {
		action->debug_gui();
	}
	ImGui::End();
}
#endif // DEBUG_FEATURES_ENABLE

void RemotePlayer::set_target(Reference<IEntity> entity) {
	selectionTarget = entity;
}
