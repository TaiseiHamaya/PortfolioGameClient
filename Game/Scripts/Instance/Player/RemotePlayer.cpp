#include "RemotePlayer.h"

#include <Library/Utility/Tools/Functions.h>

#include "../IEntity/Actions/JumpAction.h"
#include "Actions/PaladinHolySpirit.h"

using namespace std::chrono;
using clock_type = system_clock;
using time_float = duration<float, seconds::period>;

void RemotePlayer::initialize(const std::filesystem::path& file, u64 localId_) {
	IEntity::initialize(file, localId_);
	// プレイヤーのアクションを登録
	auto jumpAction = std::make_unique<JumpAction>();
	jumpAction->setup(this, std::format("{}.gltf-{}", file.stem().string(), "Dash"));
	actionList.emplace("Jump", std::move(jumpAction));
	auto paladinHolySpirit = std::make_unique<PaladinHolySpirit>();
	paladinHolySpirit->setup(this, std::format("{}.gltf-{}", file.stem().string(), "AttackSky"));
	actionList.emplace("PaladinHolySpirit", std::move(paladinHolySpirit));

	startTime = clock_type::now();
}

void RemotePlayer::update() {
	// 今の位置を記録
	Vector3 dest = transform.get_translate();
	// 移動補完
	calculate_position();

	// ----- 回転 -----
	Vector3 diff = transform.get_translate() - dest;
	Vector3 xzDiff = Vector3{ diff.x, 0.0f,diff.z };
	if (xzDiff.length() >= 0.01f) { // 十分に移動している
		Vector3 xzDirection = xzDiff.normalize();
		// xz方向の向いている方向
			// 向く方向
		Quaternion forwardTo{ Quaternion::LookForward(xzDirection.normalize()) };
		// Slerp補完
		transform.set_quaternion(
			Quaternion::Slerp(transform.get_quaternion(), forwardTo, 0.1f)
		);
	}

	IEntity::update();
}

/// <summary>
/// 
/// </summary>
/// <param name="time">サーバー時刻</param>
/// <param name="position">位置</param>
void RemotePlayer::move_to(const clock_type::time_point& time, const Vector3& position) {
	if (fixedTime.time_since_epoch().count() == 0) {
		// 初回
		fixedTime = time;
		latency.set(duration_cast<time_float>(startTime - fixedTime).count());
	}
	clock_type::duration timeWaypoint;
	timeWaypoint = time - fixedTime;
	waypoints.emplace_back(
		Waypoint{
			timeWaypoint,
			position
		}
	);
}

void RemotePlayer::calculate_position() {
	auto now = clock_type::now();

	std::optional<Vector3> newPos;
	r32 t = duration_cast<time_float>(now - startTime).count() - latency;
	u32 index = waypointIndex;
	while (index + interval < static_cast<u32>(waypoints.size())) {
		Waypoint& from = waypoints[index];
		Waypoint& to = waypoints[index + interval - 1u];

		r32 fromTime = duration_cast<time_float>(from.timestamp).count();
		r32 toTime = duration_cast<time_float>(to.timestamp).count();
		if (t >= fromTime && t < toTime) { // 補間区間内
			r32 param = eps::lerp_inv(fromTime, toTime, t);
			newPos = eps::lerp(from.position, to.position, param);
			// indexを-1する(0以上)
			waypointIndex = std::max(index, 1u) - 1;
			// 1以上の場合、古いウェイポイントを削除
			if (waypointIndex > 0) {
				waypoints.pop_front();
				// レイテンシを調整
				latency.back();
				latency.set(std::max<r32>(latency, 0.0f));
			}
			break;
		}
		else if (t < fromTime) {
			// 補間より早い
			latency.back();
			// 再計算
			t = duration_cast<time_float>(now - startTime).count() - latency;
		}
		else {
			++index;
		}
	}

	// 遅れてる
	if (index + interval >= static_cast<u32>(waypoints.size())) {
		latency.ahead();
	}

	// 補間係数
	constexpr r32 lerpParam = 0.1f;
	if (newPos.has_value()) {
		// 移動
		transform.set_translate(
			eps::lerp(
				transform.get_translate(),
				newPos.value(),
				lerpParam
			)
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
