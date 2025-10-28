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

	globalCoolDownTimer.set(0.0f);
}

void Player::update() {
	globalCoolDownTimer.back();
	IEntity::update();
}

void Player::move_to([[maybe_unused]] const std::chrono::system_clock::time_point& time, const Vector3& position) {
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

bool Player::can_play_action(eps::string_hashed actionName) const noexcept {
	if (!actionList.contains(actionName)) {
		return false;
	}
	if (currentAction && !currentAction->can_transition()) {
		return false;
	}
	Reference<IActionBasic> action = actionList.at(actionName);
	if (!action) {
		return false;
	}
	if (is_global_skill(actionName) && !is_ready_global_skill()) {
		return false;
	}
	return true;
}

bool Player::is_global_skill(eps::string_hashed actionName) const noexcept {
	if(!actionList.contains(actionName)) {
		return false;
	}
	Reference<IActionBasic> action = actionList.at(actionName);
	if (!action) {
		return false;
	}
	return (action->action_type() == ActionType::WeaponSkill || action->action_type() == ActionType::Spell);
}

bool Player::is_ready_global_skill() const noexcept {
	return globalCoolDownTimer <= 0;
}

void Player::execute_global_skill() noexcept {
	if (!is_ready_global_skill()) {
		return;
	}
	globalCoolDownTimer.set(2.5f);
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
