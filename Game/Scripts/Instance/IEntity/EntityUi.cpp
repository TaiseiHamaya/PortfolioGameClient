#include "EntityUi.h"

#include <Engine/Runtime/Scene/World/WorldRoot.h>

#include "Scripts/Extension/Util/LookAtRect.h"

void EntityUi::initialize(float offsetY, ColorRGBA color) {
	nametag = world_root_mut()->instantiate<szg::StringRectInstance>(this);
	nametag->initialize("UDEVGothic35HS-Regular.mtsdf", 0.6f, Vector2{0.5f, 1.0f});
	nametag->transform_mut().set_translate_y(offsetY);
	nametag->set_layer(1);

	hpRect = world_root_mut()->instantiate<szg::Rect3d>(nametag);
	hpRect->initialize({ 1.3f, 0.1f }, CVector2::BASIS_X);
	hpRect->transform_mut().set_translate({ 0.65f, -0.3f, 0.0f });
	hpRect->set_layer(1);
	szg::PrimitiveMaterial& material = hpRect->get_material();
	material.lightingType = LighingType::None;
	material.color = color;
	material.texture = szg::TextureLibrary::GetTexture("White.png");
}

void EntityUi::set_name(const std::string& name) {
	nametag->reset_string(name);
}

void EntityUi::update_affine() {
	WorldInstance::update_affine();

	Vector3 cameraBackward = CVector3::BACKWARD * LookAtRect::camera->world_affine().get_basis().to_quaternion();

	nametag->transform_mut().set_quaternion(
		world_affine().get_basis().to_quaternion().inverse() * Quaternion::LookForward(cameraBackward)
	);
}

void EntityUi::update_ui(r32 percentage) {
	hpRect->initialize({ 1.3f * percentage, 0.1f }, CVector2::BASIS_X);
}
