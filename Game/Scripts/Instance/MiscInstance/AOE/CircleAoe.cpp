#include "CircleAoe.h"

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/World/WorldManager.h>
#include <Library/Math/Definition.h>

CircleAoe::CircleAoe() = default;
CircleAoe::~CircleAoe() = default;

void CircleAoe::initialize(const Vector3& position, float radius, float duration_) {
	transform.set_translate(position);
	transform.set_translate_y(0.02f);

	duration = duration_;

	Quaternion rotate = Quaternion::AngleAxis(CVector3::BASIS_X, -PI / 2);

	base = world_manager()->create<Rect3d>(this);
	base->initialize(CVector3::BASIS * radius, CVector3::BASIS * 0.5f);
	base->get_material().texture = TextureLibrary::GetTexture("AOEBase.png");
	base->get_material().lightingType = LighingType::None;
	base->get_material().color.alpha = 0;
	base->get_transform().set_scale(CVector3::ZERO);
	base->get_transform().set_quaternion(rotate);
	//base->set_layer(1);
	effect = world_manager()->create<Rect3d>(this);
	effect->initialize(CVector3::BASIS * radius, CVector3::BASIS * 0.5f);
	effect->get_material().texture = TextureLibrary::GetTexture("AOEBase.png");
	effect->get_material().lightingType = LighingType::None;
	effect->get_material().color.alpha = 0;
	effect->get_transform().set_scale(CVector3::ZERO);
	effect->get_transform().set_quaternion(rotate);
	//effect->set_layer(1);
}

void CircleAoe::start(Reference<Rect3dDrawManager> rectDraw) {
	rectDraw->register_instance(base);
	rectDraw->register_instance(effect);
}

void CircleAoe::end(Reference<Rect3dDrawManager> rectDraw) {
	rectDraw->unregister_instance(base);
	rectDraw->unregister_instance(effect);

	Reference<WorldManager> worldManager= world_manager();
	worldManager->erase(base);
	worldManager->erase(effect);
}

void CircleAoe::update() {
	timer.ahead();

	if (timer * 3.0f < 1.0f) {
		float param = std::clamp(timer * 3.0f, 0.0f, 1.0f);
		base->get_transform().set_scale(Vector3::Lerp(CVector3::ZERO, CVector3::BASIS, param));
		base->get_material().color.alpha = param * 0.5f;
	}
	else {
		float time = timer - 0.33f;
		float param = time - std::trunc(time);
		base->get_transform().set_scale(CVector3::BASIS);
		effect->get_transform().set_scale(CVector3::BASIS * param);
		effect->get_material().color.alpha = (1 - param) * 0.5f;
	}

	if (timer >= duration) {
		float param = std::lerp(1.0f, 0.0f, (timer - duration) * 3.0f);
		base->get_material().color.alpha = param * 0.5f;
		effect->get_material().color.alpha = param * 0.5f;
	}
}

bool CircleAoe::is_end() const {
	return timer >= duration;
}
