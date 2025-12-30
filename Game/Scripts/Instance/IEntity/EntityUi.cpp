#include "EntityUi.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

#include "Scripts/Extension/Util/LookAtRect.h"

void EntityUi::initialize(float offsetY, Color4 color) {
	nametag = world_root_mut()->instantiate<szg::StringRectInstance>(this);
	nametag->initialize("UDEVGothic35HS-Regular.mtsdf", 8.0f, Vector2{ 0.5f,0.5f });
	nametag->transform_mut().set_translate_y(offsetY);
	nametag->set_layer(1);

	hpRect = world_root_mut()->instantiate<szg::Rect3d>(nametag);
	hpRect->initialize({ 1.3f, 0.1f }, CVector2::BASIS_X);
	hpRect->transform_mut().set_translate({ 0.65f, -0.3f, 0.0f });
	hpRect->set_layer(1);
	szg::PrimitiveMaterial& material = hpRect->get_material();
	material.lightingType = LighingType::None;
	material.color = color;
	material.texture = szg::TextureLibrary::GetTexture("white.png");
}

void EntityUi::set_name(const std::string& name) {
	nametag->reset_string(name + ' ');
}

void EntityUi::update_affine() {
	WorldInstance::update_affine();

	nametag->look_at(LookAtRect::camera);
}

void EntityUi::update_ui(r32 percentage) {
	hpRect->initialize({ 1.3f * percentage, 0.1f }, CVector2::BASIS_X);
}
