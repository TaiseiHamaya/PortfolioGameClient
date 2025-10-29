#pragma once

#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/Instance/IEntity/IEntity.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player final : public IEntity {
public:
	Player() noexcept = default;
	~Player() noexcept = default;

	__CLASS_NON_COPYABLE(Player)

public:
	void initialize(const std::filesystem::path& file, u64 localId_) override;

	void update() override;

	void move_to(const std::chrono::system_clock::time_point& time, const Vector3& position) override;

public:
	bool can_play_action(eps::string_hashed actionName) const noexcept;

	bool is_global_skill(eps::string_hashed actionName) const noexcept;
	bool is_ready_global_skill() const noexcept;
	void execute_global_skill() noexcept;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	WorldTimer globalCoolDownTimer;
};
