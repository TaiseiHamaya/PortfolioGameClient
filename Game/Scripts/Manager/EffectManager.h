#pragma once

#include <memory>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/MiscInstance/Effects/IEffectInstance.h"

class EffectManager {
public:
	EffectManager() = default;
	~EffectManager() = default;

	__CLASS_NON_COPYABLE(EffectManager)

public:
	void setup(Reference<StaticMeshDrawManager> meshDrawManager_, Reference<Rect3dDrawManager> rectDrawManager_);
	void update();
	void draw_particle();

	void register_instance(std::unique_ptr<IEffectInstance> instance);

#ifdef DEBUG_FEATURES_ENABLE
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<StaticMeshDrawManager> meshDrawManager;
	Reference<Rect3dDrawManager> rectDrawManager;
	std::vector<std::unique_ptr<IEffectInstance>> instances;
};
