#include "Player.h"

#include "../IEntity/Actions/JumpAction.h"
#include "Actions/PaladinHolySpirit.h"

void Player::initialize(const std::filesystem::path& file) {
	IEntity::initialize(file);
	// プレイヤーのアクションを登録
	auto jumpAction = std::make_unique<JumpAction>();
	jumpAction->setup(this, std::format("{}.gltf-{}", file.stem().string(), "Dash"));
	actionList.emplace("Jump", std::move(jumpAction));
	auto paladinHolySpirit = std::make_unique<PaladinHolySpirit>();
	paladinHolySpirit->setup(this, std::format("{}.gltf-{}", file.stem().string(), "AttackSky"));
	actionList.emplace("PaladinHolySpirit", std::move(paladinHolySpirit));
}

void Player::move_to([[maybe_unused]] const std::chrono::steady_clock::time_point& time, const Vector3& position) {
	if (!is_active()) {
		return;
	}

	// ---------- 移動処理 ----------
	Vector3 dest = transform.get_translate();
	transform.set_translate(position);
	Vector3 diff = position - dest;
	// ---------- 移動方向を向く ----------
	Vector3 direction = diff.normalize_safe();
	Vector3 xzDirection = Vector3{ direction.x, 0.0f,direction.z };
	if (xzDirection.length() != 0) {
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
void Player::debug_gui() {
	ImGui::Begin("Player");
	for (auto& action : actionList | std::views::values) {
		action->debug_gui();
	}
	ImGui::End();
}
#endif // DEBUG_FEATURES_ENABLE

void Player::set_target(Reference<IEntity> entity) {
	selectionTarget = entity;
}
