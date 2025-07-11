#include "IEntity.h"

#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#define COLOR4_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

#include "Actions/IdleAction.h"

using namespace std::literals::string_literals;

void IEntity::initialize(const std::filesystem::path& file) {
	JsonAsset json{ std::format(L".\\Game\\Resources\\Json\\Entity\\{}", file.native()) };
	shadow = world_manager()->create<Shadow>();
	ui = world_manager()->create<EntityUi>(this);
	ui->initialize(2.5f, json.try_emplace<Color4>("HPColor"));

	auto idleAction = std::make_unique<IdleAction>();
	idleAction->setup(this, std::format("{}.gltf-{}", file.stem().string(), "Idle"));
	actionList.emplace("Idle", std::move(idleAction));

	reset_animated_mesh(json.try_emplace<std::string>("Model"));

	targetRadius = json.try_emplace<float>("TargetRadius");
}

void IEntity::start(Reference<SkinningMeshDrawManager> skinDraw, Reference<Rect3dDrawManager> rectDraw) {
	shadow->start(this, 2.0f);
	ui->start(rectDraw);
	skinDraw->register_instance(this);
	//staticDraw->register_instance()
	rectDraw->register_instance(shadow);
}

void IEntity::begin() {
	if (!currentAction) {
		start_action("Idle");
	}

	SkinningMeshInstance::begin();
}

void IEntity::update() {
	if (currentAction) {

		// 移動が停止するようなアクションでは実行しない
		if (currentAction->action_effect() != ActionEffect::Stack) {
			// 重力処理
			velocity.y += -20.0f * WorldClock::DeltaSeconds();
			// 地面の上では摩擦減衰
			if (transform.get_translate().y == 0 && velocity.length() != 0) {
				velocity *= 0.75f;
			}
			// 移動
			transform.plus_translate(velocity * WorldClock::DeltaSeconds());

			// 地面に埋まらないようにする
			if (transform.get_translate().y <= 0.0f) {
				transform.set_translate_y(0.0f);
			}
		}

		currentAction->update();

		if (currentAction->end_action()) {
			start_action("Idle");
		}
	}

	Vector3 xzVelocity = Vector3{ velocity.x, 0.0f,velocity.z };
	// ---------- 移動方向を向く ----------
	if (xzVelocity.length() != 0) {
		// 向く方向
		Quaternion forwardTo{ Quaternion::LookForward(xzVelocity.normalize()) };
		// Slerp補完
		transform.set_quaternion(
			Quaternion::Slerp(transform.get_quaternion(), forwardTo, 0.1f)
		);
	}

	update_animation();
	shadow->update();
	ui->update();
}

void IEntity::start_action(eps::string_hashed actionName) {
	if (actionList.contains(actionName)) {
		currentAction = actionList[actionName];
		currentAction->reset();
		currentAction->reset_animation();
		currentAction->start();
	}
}

void IEntity::move(const Vector2& xzDirection) {
	if (!is_active()) {
		return;
	}
	Vector2 xzVelocity = xzDirection * SPEED;
	velocity = Vector3{ xzVelocity.x, velocity.y,xzVelocity.y };
}

void IEntity::jump() {
	if (!is_active()) {
		return;
	}
	// ---------- ジャンプ処理 ----------
	if (transform.get_translate().y == 0) {
		velocity.y = 8.0f;
	}
	start_action("Jump");
}

void IEntity::on_damaged(i32 damage) {
	hitpoint -= damage;
	start_action("Damaged");
}

r32 IEntity::target_radius() const {
	return targetRadius;
}

Reference<IEntity> IEntity::get_selection_target() const {
	return selectionTarget;
}
