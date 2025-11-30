#include "PaladinHolySpirit.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

#include "PaladinHolySpiritEffectSelf.h"
#include "PaladinHolySpiritEffectTarget.h"

PaladinHolySpirit::PaladinHolySpirit() noexcept = default;
PaladinHolySpirit::~PaladinHolySpirit() noexcept = default;

void PaladinHolySpirit::setup(Reference<IEntity> owner_, const std::string& animationName) {
	IActionBasic::setup(owner_, animationName);
	load_from_json("Action/Player/Paladin/HolySpirit.json");

#ifdef DEBUG_FEATURES_ENABLE
	effectTarget = std::make_unique<PaladinHolySpiritEffectTarget>();
	effectTarget->load_constant_values();
#endif // DEBUG_FEATURES_ENABLE
}

void PaladinHolySpirit::start() {
	auto temp = owner->world_root_mut()->instantiate<PaladinHolySpiritEffectTarget>();
	temp->initialize(EffectTools::CalculateTargetPosition(owner, owner->get_selection_target()));
	effectManager->register_instance(std::move(temp));
}

void PaladinHolySpirit::update() {
	timer.ahead();
}

void PaladinHolySpirit::reset() {
	timer.set(0);
}

bool PaladinHolySpirit::can_transition() const {
	return timer >= castTime + 0.3f;
}

bool PaladinHolySpirit::end_action() const {
	return timer >= recastTime;
}


#ifdef DEBUG_FEATURES_ENABLE

void PaladinHolySpirit::debug_gui() {
	// enumはラジオボタン
	if (ImGui::TreeNode("PaladinHolySpirit")) {
		ISkillAction::debug_gui();

		if (ImGui::TreeNode("Effect")) {

			effectTarget->debug_gui();

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

#endif // DEBUG_FEATURES_ENABLE
