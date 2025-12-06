#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>

/// <summary>
/// 影描画用クラス
/// </summary>
class Shadow final : public szg::Rect3d {
public:
	void setup(Reference<szg::WorldInstance> owner_, float baseRadius);
	void fixed_update() override;

private:
	Reference<szg::WorldInstance> owner;
	float baseRadius;
};
