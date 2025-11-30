#include "EntityUi.h"

#include <Engine/Module/World/WorldManager.h>

void EntityUi::initialize(float offsetY, Color4 color) {
	hpRect = world_manager()->create<LookAtRect>(this);
	hpRect->initialize({ 1.3f, 0.1f }, { 0.5f,0.5f });
	hpRect->get_transform().set_translate_y(offsetY);
	PrimitiveMaterial& material = hpRect->get_material();
	material.lightingType = LighingType::None;
	material.color = color;
	material.texture = TextureLibrary::GetTexture("white.png");

	nametag = world_manager()->create<StringRectInstance>(hpRect);
	nametag->initialize("UDEVGothic35HS-Regular.mtsdf", 8.0f, Vector2{ 0.5f,0.5f });
	nametag->get_transform().set_translate_y(0.5f);
}

void EntityUi::start(Reference<Rect3dDrawManager> rectDraw, Reference<StringRectDrawManager> stringDraw) {
	rectDraw->register_instance(hpRect);
	stringDraw->register_instance(nametag);
}

void EntityUi::terminate(Reference<Rect3dDrawManager> rectDraw, Reference<StringRectDrawManager> stringDraw) {
	rectDraw->unregister_instance(hpRect);
	stringDraw->unregister_instance(nametag);
	world_manager()->erase(hpRect);
	world_manager()->erase(nametag);
}

void EntityUi::set_name(const std::string& name) {
	nametag->set_string(name);
}
