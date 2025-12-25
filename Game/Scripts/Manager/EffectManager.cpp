#include "EffectManager.h"

void EffectManager::prev_update() {
}

void EffectManager::post_update() {
	std::erase_if(instances,
		[&](Reference<IEffectInstance>& instance) {
		if (instance->is_end_effect()) {
			instance->destroy_self();
			return true;
		}
		return false;
	});
}

void EffectManager::draw_particle() {
	for (const Reference<IEffectInstance>& instance : instances) {
		instance->draw_particle();
	}
}

void EffectManager::register_instance(Reference<IEffectInstance> instance) {
	if (!instance) {
		return;
	}

	instances.emplace_back(std::move(instance));
}

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>

void EffectManager::debug_gui() {
	ImGui::Text("Count : %d", instances.size());
	if (instances.empty()) {
		return;
	}
	ImGui::Separator();

	instances[0]->debug_gui();
}
#endif // DEBUG_FEATURES_ENABLE
