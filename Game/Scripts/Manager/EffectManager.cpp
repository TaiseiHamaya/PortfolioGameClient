#include "EffectManager.h"

void EffectManager::setup(Reference<StaticMeshDrawManager> meshDrawManager_, Reference<Rect3dDrawManager> rectDrawManager_) {
	meshDrawManager = meshDrawManager_;
	rectDrawManager = rectDrawManager_;
}

void EffectManager::update() {
	for (std::unique_ptr<IEffectInstance>& instance : instances) {
		instance->update();
	}

	std::erase_if(instances,
		[&](const std::unique_ptr<IEffectInstance>& instance) {
		if (instance->is_destroy()) {
			instance->terminate(meshDrawManager, rectDrawManager);
			return true;
		}
		return false;
	});
}

void EffectManager::draw_particle() {
	for (const std::unique_ptr<IEffectInstance>& instance : instances) {
		instance->draw_particle();
	}
}

void EffectManager::register_instance(std::unique_ptr<IEffectInstance> instance) {
	if (!instance) {
		return;
	}

	instance->setup(meshDrawManager, rectDrawManager);
	instances.emplace_back(std::move(instance));
}

#ifdef DEBUG_FEATURES_ENABLE
void EffectManager::debug_gui() {
	ImGui::Text("Count : %d", instances.size());
	if (instances.empty()) {
		return;
	}
	ImGui::Separator();

	instances[0]->debug_gui();
}
#endif // DEBUG_FEATURES_ENABLE
