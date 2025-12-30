#pragma once

#include <Engine/Module/World/WorldInstance/WorldInstance.h>

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

#include "Scripts/Instance/IEntity/IEntity.h"

namespace EffectTools {

Vector3 CalculateTargetPosition(Reference<const szg::WorldInstance> self, Reference<const IEntity> target);

}

/// <summary>
/// エフェクトインターフェース
/// </summary>
class IEffectInstance : public szg::WorldInstance {
public:
	IEffectInstance() = default;
	virtual ~IEffectInstance() = default;

	SZG_CLASS_MOVE_ONLY(IEffectInstance)

public:
	virtual void draw_particle() const = 0;

	bool is_end_effect() const;

protected:
	bool isEnded{ false };

public:
#ifdef DEBUG_FEATURES_ENABLE
	virtual void debug_gui() = 0;
#endif // DEBUG_FEATURES_ENABLE
};
