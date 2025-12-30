#include "EntityUi.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

void EntityUi::initialize(float offsetY, Color4 color) {
	hpRect = world_root_mut()->instantiate<LookAtRect>(this);
	hpRect->initialize({ 1.3f, 0.1f }, { 0.5f,0.5f });
	hpRect->transform_mut().set_translate_y(offsetY);
	hpRect->set_layer(1);
	szg::PrimitiveMaterial& material = hpRect->get_material();
	material.lightingType = LighingType::None;
	material.color = color;
	material.texture = szg::TextureLibrary::GetTexture("white.png");

	nametag = world_root_mut()->instantiate<szg::StringRectInstance>(hpRect);
	nametag->initialize("UDEVGothic35HS-Regular.mtsdf", 8.0f, Vector2{ 0.5f,0.5f });
	nametag->transform_mut().set_translate_y(0.3f);
	nametag->set_layer(1);
}

void EntityUi::set_name(const std::string& name) {
	nametag->reset_string(name + ' ');
}
