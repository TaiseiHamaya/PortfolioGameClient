#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>

/// <summary>
/// HPバーのWorldInstance
/// </summary>
class EntityUi final : public szg::WorldInstance {
public:
	void initialize(float offsetY, ColorRGBA color);

public:
	void set_name(const std::string& name);

	void update_affine() override;

	void update_ui(r32 percentage);

private:
	Reference<szg::Rect3d> hpRect;
	Reference<szg::StringRectInstance> nametag;
};
