#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <memory>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Instance/MiscInstance/Effects/IEffectInstance.h"

/// <summary>
/// エフェクト管理クラス
/// </summary>
class EffectManager final : public szg::ISceneScript {
public:
	EffectManager() = default;
	~EffectManager() = default;

	SZG_CLASS_MOVE_ONLY(EffectManager)

public:
	void prev_update() override;

	void post_update() override;

	void update();
	void draw_particle();

	void register_instance(Reference<IEffectInstance> instance);

#ifdef DEBUG_FEATURES_ENABLE
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	std::vector<Reference<IEffectInstance>> instances;
};
