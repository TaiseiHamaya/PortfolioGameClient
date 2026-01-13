#include "DamageNumberEffect.h"

#include <Library/Utility/Tools/MathEPS.h>

#include <Engine/Module/Manager/World/WorldRoot.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>
#include <Engine/Runtime/Clock/WorldClock.h>

void DamageNumberEffect::initialize(i32 damage, ColorRGBA color) {
	damageNumber = world_root_mut()->instantiate<szg::StringRectInstance>(this);

	damageNumber->initialize("UDEVGothic35HS-Regular.mtsdf", 12.0f, CVector2::ONE / 2);
	damageNumber->reset_string(std::to_string(damage));
	damageNumber->get_material().color = color;
	damageNumber->get_material().color.alpha = 1.0f;
	damageNumber->set_layer(2);
}

void DamageNumberEffect::update() {
	timer.ahead();
	if (timer >= OpaqueTime && timer < EffectTime) {
		damageNumber->get_material().color.alpha = eps::lerp(
			1.0f, 0.0f, (timer - OpaqueTime) / (EffectTime - OpaqueTime)
		);
	}
	else if (timer >= EffectTime) {
		isEnded = true;
	}

	Vector3 velocity{ 0.0f, 1.0f, 0.0f };
	transform.plus_translate(velocity * szg::WorldClock::DeltaSeconds());
}

void DamageNumberEffect::update_affine() {
	IEffectInstance::update_affine();
	if (damageNumber) {
		damageNumber->look_at(cameraInstance);
	}
}
