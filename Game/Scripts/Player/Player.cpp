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

#ifdef _DEBUG
#include <imgui.h>
void Player::debug_gui() {
	ImGui::Begin("Player");
	SkinningMeshInstance::debug_gui();

	for (auto& action : actionList | std::views::values) {
		action->debug_gui();
	}

	ImGui::End();
}
#endif // _DEBUG

void Player::set_target(Reference<IEntity> entity) {
	selectionTarget = entity;
}
