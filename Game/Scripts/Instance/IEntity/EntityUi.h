#pragma once

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/StringRectDrawManager.h>

#include "Scripts/Extension/Util/LookAtRect.h"
#include "Engine/Module/World/Mesh/Primitive/StringRectInstance.h"

/// <summary>
/// HPバーのWorldInstance
/// </summary>
class EntityUi final : public WorldInstance {
public:
	void initialize(float offsetY, Color4 color);
	void start(Reference<Rect3dDrawManager> rectDraw, Reference<StringRectDrawManager> stringDraw);
	void terminate(Reference<Rect3dDrawManager> rectDraw, Reference<StringRectDrawManager> stringDraw);

public:
	void set_name(const std::string& name);

private:
	std::unique_ptr<LookAtRect> hpRect;
	std::unique_ptr<StringRectInstance> nametag;
};
