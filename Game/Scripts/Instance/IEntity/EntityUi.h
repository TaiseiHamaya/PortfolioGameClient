#pragma once

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/StringRectDrawManager.h>

#include "Scripts/Extension/Util/LookAtRect.h"
#include "Engine/Module/World/Mesh/Primitive/StringRectInstance.h"

/// <summary>
/// HPバーのWorldInstance
/// </summary>
class EntityUi final : public szg::WorldInstance {
public:
	void initialize(float offsetY, Color4 color);

public:
	void set_name(const std::string& name);

private:
	Reference<LookAtRect> hpRect;
	Reference<szg::StringRectInstance> nametag;
};
