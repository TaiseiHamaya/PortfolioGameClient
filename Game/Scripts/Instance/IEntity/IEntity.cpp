#include "IEntity.h"

#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Clock/WorldClock.h>
#define COLOR4_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

#include "Actions/IdleAction.h"

using namespace std::literals::string_literals;

void IEntity::initialize(const std::filesystem::path& file, u64 localId_) {
	localId = localId_;

	JsonAsset json{ std::format(L".\\Game\\Resources\\Json\\Entity\\{}", file.native()) }; // Jsonからデータの読み込み
	// Instance生成
	shadow = world_manager()->create<Shadow>();
	ui = world_manager()->create<EntityUi>(this);
	ui->initialize(2.5f, json.try_emplace<Color4>("HPColor"));

	// アクションデータの生成
	auto idleAction = std::make_unique<IdleAction>();
	idleAction->setup(this, std::format("{}.gltf-{}", file.stem().string(), "Idle"));
	actionList.emplace("Idle", std::move(idleAction));
	// メッシュの設定
	reset_animated_mesh(json.try_emplace<std::string>("Model"));

	targetRadius = json.try_emplace<float>("TargetRadius");
}

void IEntity::setup(Reference<SkinningMeshDrawManager> skinDraw, Reference<Rect3dDrawManager> rectDraw) {
	// 描画の登録
	shadow->setup(this, 2.0f);
	ui->start(rectDraw);
	skinDraw->register_instance(this);
	//staticDraw->register_instance()
	rectDraw->register_instance(shadow);
}

void IEntity::begin() {
	// 実行中のアクションが存在しない場合Idleにする
	if (!currentAction) {
		start_action("Idle");
	}

	SkinningMeshInstance::begin();
}

void IEntity::terminate(Reference<SkinningMeshDrawManager> skinDraw, Reference<Rect3dDrawManager> rectDraw) {
	// 描画登録の解除
	skinDraw->unregister_instance(this);
	rectDraw->unregister_instance(shadow);
	ui->terminate(rectDraw);
	// WorldManager削除
	world_manager()->erase(shadow);
	world_manager()->erase(ui);
	world_manager()->erase(this);
}

void IEntity::update() {
	if (currentAction) {
		// 移動が停止するようなアクションでは実行しない
		if (currentAction->action_effect() != ActionEffect::Stack) {
			// 重力処理
			velocityY += -20.0f * WorldClock::DeltaSeconds();
			// 地面に埋まらないようにする
			if (transform.get_translate().y <= 0.0f) {
				transform.set_translate_y(0.0f);
			}
		}

		currentAction->update();
		// アクションが終了したらIdleに戻す
		if (currentAction->end_action()) {
			start_action("Idle");
		}
	}

	update_animation();
	shadow->update();
	ui->update();
}

void IEntity::start_action(eps::string_hashed actionName) {
	if (!actionList.contains(actionName)) { // アクションが存在しない
		return;
	}
	if (currentAction && !currentAction->can_transition()) { // 遷移できない
		return;
	}
	Reference<IActionBasic> action = actionList[actionName];
	if (!action) { // nullチェック
		return;
	}
	// 実行
	currentAction = action;
	currentAction->reset();
	currentAction->reset_animation();
	currentAction->start();
}

void IEntity::jump() {
	if (!is_active()) {
		return;
	}
	// ---------- ジャンプ処理 ----------
	if (transform.get_translate().y == 0) {
		velocityY = 8.0f;
	}
	start_action("Jump");
}

void IEntity::on_damaged(i32 damage) {
	hitpoint -= damage;
	start_action("Damaged");
}

const std::vector<u64>& IEntity::get_enmity_ids() const noexcept {
	return enmityIds;
}

u64 IEntity::local_id() const noexcept {
	return localId;
}

const std::optional<u64>& IEntity::server_id() const noexcept {
	return serverId;
}

std::optional<u64>& IEntity::server_id_mut() noexcept {
	return serverId;
}

const std::string& IEntity::name_imm() const noexcept {
	return name;
}

r32 IEntity::target_radius() const noexcept {
	return targetRadius;
}

Reference<IEntity> IEntity::get_selection_target() const noexcept {
	return selectionTarget;
}

Reference<IActionBasic> IEntity::now_action() const noexcept {
	return currentAction;
}

void IEntity::set_target(Reference<IEntity> entity) noexcept {
	selectionTarget = entity;
}

void IEntity::set_server_id(u64 id) {
	serverId = id;
}

void IEntity::set_name(const std::string& name_) {
	name = name_;
}
