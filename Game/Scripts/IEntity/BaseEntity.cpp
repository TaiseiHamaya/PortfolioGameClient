#include "BaseEntity.h"

#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#include <Engine/Module/World/WorldManager.h>
#define COLOR4_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

#include "Actions/Idle.h"

using namespace std::literals::string_literals;

void BaseEntity::initialize([[maybe_unused]] const char* name) {
	JsonAsset json{ std::format(".\\Game\\Resources\\Json\\Entity\\{}.json", name)};
	shadow = world_manager()->create<Shadow>();
	ui = world_manager()->create<EntityUi>(this);
	ui->initialize(2.5f, json.try_emplace<Color4>("HPColor"));

	idleAction = std::make_unique<Idle>();
	idleAction->start(this, std::format("{}.gltf-{}", name, "Idle"));

	reset_animated_mesh(json.try_emplace<std::string>("Model"));
	set_action(idleAction);
}

void BaseEntity::start(Reference<SkinningMeshDrawManager> skinDraw, Reference<Rect3dDrawManager> rectDraw) {
	shadow->start(this, 2.0f);
	ui->start(rectDraw);
	skinDraw->register_instance(this);
	//staticDraw->register_instance()
	rectDraw->register_instance(shadow);
}

void BaseEntity::begin() {
	if (!nowAction) {
		set_action(idleAction);
	}

	nowAction->begin();
}

void BaseEntity::update() {
	if (!nowAction) {
		return;
	}
	// 移動が停止するようなアクションでは実行しない
	if (nowAction->action_effect() != ActionEffect::Stack) {
		// 重力処理
		velocity.y += -20.0f * WorldClock::DeltaSeconds();
		// 地面の上では摩擦減衰
		if (transform.get_translate().y == 0 && velocity.length() != 0) {
			velocity *= 0.75f;
		}
		// 移動
		transform.plus_translate(velocity* WorldClock::DeltaSeconds());

		// 地面に埋まらないようにする
		if (transform.get_translate().y <= 0.0f) {
			transform.set_translate_y(0.0f);
		}
	}

	nowAction->update();

	Vector3 xzDirection = Vector3{ velocity.x, 0.0f,velocity.z }.normalize_safe();
	// ---------- 移動方向を向く ----------
	// 向く方向
	Quaternion forwardTo{ transform.get_quaternion() };
	if (xzDirection.length() != 0) {
		forwardTo = Quaternion::LookForward(xzDirection.normalize());
	}
	// Slerp補完
	transform.set_quaternion(
		Quaternion::Slerp(transform.get_quaternion(), forwardTo, 0.1f)
	);

	if (nowAction->end_action()) {
		set_action(idleAction);
	}

	update_animation();
	shadow->update();
	ui->update();
}

void BaseEntity::start_action(u32 index) {
	if (nowAction && nowAction->progress()) {
		set_action(actionList[index]);
	}
}

void BaseEntity::move(const Vector2& xzDirection) {
	if (!is_active()) {
		return;
	}
	Vector2 xzVelocity = xzDirection * SPEED;
	velocity = Vector3{ xzVelocity.x, velocity.y,xzVelocity.y };
}

void BaseEntity::jump() {
	if (!is_active()) {
		return;
	}
	// ---------- ジャンプ処理 ----------
	if (transform.get_translate().y == 0) {
		velocity.y = 8.0f;
	}
}

void BaseEntity::on_damaged(i32 damage) {
	hitpoint -= damage;
	set_action(damagedAction);
}

void BaseEntity::sync_position(Vector3 position, r32 yAngle) {
	transform.set_translate(position);
	transform.set_quaternion(
		Quaternion::AngleAxis(CVector3::BASIS_Y, yAngle)
	);
}

void BaseEntity::set_action(Reference<BaseAction> action) {
	nowAction = action;
	nowAction->reset();
	nowAction->reset_animation();
}
