#include "RemotePlayer.h"

#include <Library/Utility/Tools/Functions.h>

#include "../IEntity/Actions/JumpAction.h"
#include "Actions/PaladinHolySpirit.h"

using clock_type = std::chrono::steady_clock;

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

void RemotePlayer::move_to([[maybe_unused]] const clock_type::time_point&, const Vector3& position) {
	clock_type::time_point now = clock_type::now();
	if (waypoints.empty()) {
		waypoints.emplace_back(now, transform.get_translate());
	}
	// 50ms後に到達するようにする
	clock_type::time_point time = std::chrono::time_point<clock_type>(now.time_since_epoch() + std::chrono::milliseconds(50));
	waypoints.emplace_back(time, position);
}

void RemotePlayer::calculate_position() {
	if (waypoints.size() < 2) {
		return;
	}

	clock_type::time_point now = clock_type::now();
	// 最初の2点を取り出す
	const auto& [firstTime, firstPosition] = waypoints.front();
	const auto& [secondTime, secondPosition] = *std::next(waypoints.begin());

	if (now < firstTime) {
		// 最初の点まで戻る
		transform.set_translate(firstPosition);
		return;
	}

	// 2点間の補完パラメータを計算
	r32 param = (now - firstTime).count() / static_cast<r32>((secondTime - firstTime).count());

	// 位置補完
	Vector3 position = eps::lerp(firstPosition, secondPosition, param);
	if (param >= 1.0f) {
		// 2点目に到達したら最初の点を削除
		waypoints.pop_front();
	}
	// 今の位置を記録
	Vector3 dest = transform.get_translate();
	// 移動
	transform.set_translate(position);

	// ----- 回転 -----
	Vector3 diff = position - dest;
	Vector3 xzDiff = Vector3{ diff.x, 0.0f,diff.z };
	if (xzDiff.length() < 0.01f) {
		// あまり動いていない
		return;
	}
	Vector3 xzDirection = xzDiff.normalize();
	// xz方向の向いている方向
		// 向く方向
	Quaternion forwardTo{ Quaternion::LookForward(xzDirection.normalize()) };
	// Slerp補完
	transform.set_quaternion(
		Quaternion::Slerp(transform.get_quaternion(), forwardTo, 0.1f)
	);
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
