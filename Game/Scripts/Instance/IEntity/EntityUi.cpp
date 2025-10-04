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
}

void EntityUi::start(Reference<Rect3dDrawManager> rectDraw) {
	rectDraw->register_instance(hpRect);
}
