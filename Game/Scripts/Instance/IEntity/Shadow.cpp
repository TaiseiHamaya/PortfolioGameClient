#include "Shadow.h"

void Shadow::setup(Reference<WorldInstance> owner_, float baseRadius_) {
	owner = owner_;
	baseRadius = baseRadius_;
	transform.set_quaternion(
		Quaternion::LookForward(CVector3::UP)
	);
	initialize(Vector2{ baseRadius,baseRadius }, Vector2{ 0.5f,0.5f });
	get_material().texture = TextureLibrary::GetTexture("shadow.png");
}

void Shadow::fixed_update() {
	// 高さに応じて影の大きさを変える
	if (!owner) {
		return;
	}
	float radiusMag = 1 / (owner->world_position().y + 2);
	initialize(Vector2{ baseRadius * radiusMag,baseRadius * radiusMag }, Vector2{ 0.5f,0.5f });
	transform.set_translate(owner->world_position());
	// 地面とのZ-Fighting対策
	transform.set_translate_y(0.01f);
}
