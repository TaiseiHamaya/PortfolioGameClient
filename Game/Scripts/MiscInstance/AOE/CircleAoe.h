#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

class Rect3dDrawManager;

class CircleAoe : public WorldInstance {
public:
	CircleAoe();
	~CircleAoe();

public:
	void initialize(const Vector3& position, float radius, float duration_);
	void start(Reference<Rect3dDrawManager> rectDraw);
	void end(Reference<Rect3dDrawManager> rectDraw);
	void update();

public:
	bool is_end() const;

private:
	WorldTimer timer;
	float duration;

	std::unique_ptr<Rect3d> base;
	std::unique_ptr<Rect3d> effect;
};
