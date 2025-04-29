#include "Player.h"

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
