#pragma once

#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Template/bitflag.h>

#include <memory>
#include <vector>

#include "BaseAction.h"
#include "EntityUi.h"
#include "Shadow.h"

enum class EntityFlag {
	Idle = 0b00001, // 通常待機
	Battle = 0b00010, // バトル状態
	Dead = 0b00100, // 死亡状態
	Invisible = 0b01000, // 不可視
	Invincible = 0b10000, // 無敵

	Targetable = Idle | Battle,
	ShowEnmityList = Battle | Invincible,
};

__USE_BITFLAG(EntityFlag)

class BaseEntity : public SkinningMeshInstance {
public:
	void initialize(const char* name);
	void start(
		Reference<SkinningMeshDrawManager> skinDraw,
		Reference<Rect3dDrawManager> rectDraw);
	virtual void begin();

	virtual void update();

public:
	void start_action(u32 index);
	void move(const Vector2& xzDirection);
	void jump();
	void on_damaged(i32 damage);
	void sync_position(Vector3 position, r32 yAngle);

protected:
	void set_action(Reference<BaseAction> action);

protected:
	u64 id;
	i32 hitpoint;

	Vector3 velocity;
	r32 SPEED{ 5.0f };

	eps::bitflag<EntityFlag> flags;

	std::unique_ptr<EntityUi> ui;
	std::unique_ptr<Shadow> shadow;

	Reference<BaseEntity> target{ nullptr }; // 対象

	Reference<BaseAction> nowAction{ nullptr }; // 今のアクション

	std::unique_ptr<BaseAction> idleAction;
	std::unique_ptr<BaseAction> fightAction;
	std::unique_ptr<BaseAction> damagedAction;
	std::vector<std::unique_ptr<BaseAction>> actionList;

public:
	Reference<BaseAction> now_action() const { return nowAction; }
};
