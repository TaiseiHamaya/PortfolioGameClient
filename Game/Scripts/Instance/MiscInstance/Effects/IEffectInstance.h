#pragma once

#include <Engine/Module/World/WorldInstance/WorldInstance.h>

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

#include "Scripts/Instance/IEntity/IEntity.h"

namespace EffectTools {

Vector3 CalculateTargetPosition(Reference<IEntity> self, Reference<IEntity> target);

}

/// <summary>
/// エフェクトインターフェース
/// </summary>
class IEffectInstance : public WorldInstance {
public:
	IEffectInstance() = default;
	virtual ~IEffectInstance() = default;

	__CLASS_NON_COPYABLE(IEffectInstance)

public:
	virtual void setup(Reference<StaticMeshDrawManager>, Reference<Rect3dDrawManager>) = 0;
	virtual void terminate(Reference<StaticMeshDrawManager>, Reference<Rect3dDrawManager>) = 0;
	virtual void draw_particle() const = 0;

	bool is_destroy() const;

protected:
	bool isDestroy{ false };

public:
#ifdef DEBUG_FEATURES_ENABLE
	virtual void debug_gui() = 0;
#endif // DEBUG_FEATURES_ENABLE
};
