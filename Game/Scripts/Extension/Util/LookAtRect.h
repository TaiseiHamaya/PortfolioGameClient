#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>

/// <summary>
/// ビルボード
/// </summary>
class LookAtRect final : public Rect3d {
public:
	void fixed_update() override;

public:
	static inline Reference<WorldInstance> camera{};
};
