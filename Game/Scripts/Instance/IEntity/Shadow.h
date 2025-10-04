#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>

class Shadow final : public Rect3d {
public:
	void setup(Reference<WorldInstance> owner_, float scale);
	void update() override;

private:
	Reference<WorldInstance> owner;
	float scale;
};
