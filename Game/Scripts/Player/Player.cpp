#include "Player.h"

Player::Player() :
	BaseEntity() {
	reset_animated_mesh("Player.gltf");
}

#ifdef _DEBUG
#include <imgui.h>
void Player::debug_gui() {
	ImGui::Begin("Player");
	SkinningMeshInstance::debug_gui();
	
	for (auto& action : actionList) {
		action->debug_gui();
	}

	ImGui::End();
}
#endif // _DEBUG
