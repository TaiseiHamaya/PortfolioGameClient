#include "Shadow.h"

void Shadow::start(Reference<WorldInstance> owner_, float scale_) {
	owner = owner_;
	scale = scale_;
	transform.set_quaternion(
		Quaternion::LookForward(CVector3::UP)
	);
	initialize(Vector2{ scale,scale }, Vector2{ 0.5f,0.5f });
	get_material().texture = TextureLibrary::GetTexture("shadow.png");
}

void Shadow::update() {
	float scaleMag = 1 / (owner->world_position().y + 2);
	initialize(Vector2{ scale * scaleMag,scale * scaleMag }, Vector2{ 0.5f,0.5f });
	transform.set_translate(owner->world_position());
	transform.set_translate_y(0.01f);
}
