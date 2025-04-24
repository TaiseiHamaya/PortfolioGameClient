#include "Enemy.h"

#include <Library/Utility/Tools/SmartPointer.h>
#include <Engine/Module/World/Collision/Collider/SphereCollider.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Library/Utility/Tools/RandomEngine.h>
#include <Engine/Module/World/WorldManager.h>

#include "Scripts/Util/RandomUtil.h"

Enemy::Enemy(Vector3 translate_) {
	reset_animated_mesh("Enemy.gltf");
	translate = translate_;
}

void Enemy::initialize() {
	shadow = world_manager()->create<Shadow>(nullptr);
}

void Enemy::begin() {
	shadow->begin();

	if (hitPoint <= 0) {
		isDead = true;
	}
}

void Enemy::update() {

	shadow->update();
	float scaleBase = 1 / (get_transform().get_translate().y + 2);
	shadow->get_transform().set_scale({ scaleBase, scaleBase, scaleBase });
	shadow->get_transform().set_translate(world_position());
	shadow->get_transform().set_translate_y(0.01f);
}

void Enemy::take_damage() {
	hitPoint -= 1.0f;
	direction = RandomOnSphere();
	Vector3 xzKnockbackDirection = (world_position() - player->world_position());
	xzKnockbackDirection.y = 0;
}
