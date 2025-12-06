#include "ISkillAction.h"

void ISkillAction::SetEffectManager(Reference<EffectManager> manager) {
	effectManager = manager;
}

void ISkillAction::load_from_json(const std::filesystem::path& file) {
	json.load(file);
	type = json.try_emplace<ActionType>("ActionType");
	targetType = json.try_emplace<TargetType>("TargetType");
	effect = json.try_emplace<ActionEffect>("ActionEffect");
	json.register_value(SZG_JSON_ASSET_REGISTER(castTime));
	json.register_value(SZG_JSON_ASSET_REGISTER(recastTime));
	json.register_value(SZG_JSON_ASSET_REGISTER(mpCost));
	json.register_value(SZG_JSON_ASSET_REGISTER(range));
	json.register_value(SZG_JSON_ASSET_REGISTER(radius));
	json.register_value(SZG_JSON_ASSET_REGISTER(loopAnimation));
	timer.set(0);
}

#ifdef DEBUG_FEATURES_ENABLE

void ISkillAction::debug_gui() {
	ImGui::Text("TargetType");
	if (ImGui::RadioButton("Target", targetType == TargetType::Target)) {
		targetType = TargetType::Target;
	}
	if (ImGui::RadioButton("Self", targetType == TargetType::Self)) {
		targetType = TargetType::Target;
	}
	if (ImGui::RadioButton("Ground", targetType == TargetType::Ground)) {
		targetType = TargetType::Ground;
	}
	ImGui::Separator();
	ImGui::Text("ActionType");
	if (ImGui::RadioButton("WeaponSkill##ActionType", type == ActionType::WeaponSkill)) {
		type = ActionType::WeaponSkill;
	}
	if (ImGui::RadioButton("Spell##ActionType", type == ActionType::Spell)) {
		type = ActionType::Spell;
	}
	if (ImGui::RadioButton("Ability##ActionType", type == ActionType::Ability)) {
		type = ActionType::Ability;
	}
	if (ImGui::RadioButton("Misc##ActionType", type == ActionType::Misc)) {
		type = ActionType::Misc;
	}

	ImGui::Separator();
	ImGui::Text("Effect");
	if (ImGui::RadioButton("None##Effect", effect == ActionEffect::None)) {
		effect = ActionEffect::None;
	}
	if (ImGui::RadioButton("Spell##Effect", effect == ActionEffect::Spell)) {
		effect = ActionEffect::Spell;
	}
	if (ImGui::RadioButton("Stack##Effect", effect == ActionEffect::Stack)) {
		effect = ActionEffect::Stack;
	}

	json.editor_gui();

	if (ImGui::Button("save")) {
		json.write("TargetType", targetType);
		json.write("ActionType", type);
		json.write("ActionEffect", effect);

		json.save();
	}
}

#endif // DEBUG_FEATURES_ENABLE
