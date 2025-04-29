#pragma once

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>

#include "Scripts/Util/LookAtRect.h"

class EntityUi final : public WorldInstance {
public:
	void initialize(float offsetY, Color4 color);
	void start(Reference<Rect3dDrawManager> rectDraw);

private:
	std::unique_ptr<LookAtRect> hpRect;
};
