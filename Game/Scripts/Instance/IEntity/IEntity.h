#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include <string>

#include <Library/Utility/Template/bitflag.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Template/string_hashed.h>

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>

#include "EntityUi.h"
#include "IActionBasic.h"
#include "Shadow.h"

enum class EntityFlag {
	Idle = 0b00001, // 通常待機
	Battle = 0b00010, // バトル状態
	Dead = 0b00100, // 死亡状態
	Invisible = 0b01000, // 不可視
	Invincible = 0b10000, // 無敵

	Default = Idle,
	Targetable = Idle | Battle,
	ShowEnmityList = Battle | Invincible,
};

__USE_BITFLAG(EntityFlag)

class IEntity : public SkinningMeshInstance {
public:
	virtual void initialize(const std::filesystem::path& file);
	void setup(
		Reference<SkinningMeshDrawManager> skinDraw,
		Reference<Rect3dDrawManager> rectDraw
	);
	virtual void begin() override;

	virtual void update() override;

public:
	void start_action(eps::string_hashed actionName);
	virtual void move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) = 0;
	void jump();
	void on_damaged(i32 damage);

protected:
	const std::vector<u64>& get_enmity_ids() const noexcept;

protected:
	u64 localId;
	std::optional<u64> serverId;
	std::string name{ "Unknown" };

	i32 hitpoint;

	r32 targetRadius{};

	r32 velocityY;
	r32 SPEED{ 5.0f };

	eps::bitflag<EntityFlag> flags;

	std::unique_ptr<EntityUi> ui;
	std::unique_ptr<Shadow> shadow;

	Reference<IEntity> selectionTarget; // 選択対象
	std::vector<u64> enmityIds; // 敵対一覧

	Reference<IActionBasic> currentAction{ nullptr }; // 今のアクション
	std::unordered_map<eps::string_hashed, std::unique_ptr<IActionBasic>> actionList;

public:
	u64 local_id() const noexcept;
	const std::optional<u64>& server_id() const noexcept;
	std::optional<u64>& server_id_mut() noexcept;
	const std::string& name_imu() const noexcept;
	r32 target_radius() const noexcept;
	Reference<IEntity> get_selection_target() const noexcept;
	Reference<IActionBasic> now_action() const noexcept;

	void set_server_id(u64 id);
	void set_name(const std::string& name);
};
