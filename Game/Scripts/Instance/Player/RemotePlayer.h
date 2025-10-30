#pragma once

#include <chrono>
#include <deque>

#include "Scripts/Instance/IEntity/IEntity.h"

/// <summary>
/// 他プレイヤー
/// </summary>
class RemotePlayer final : public IEntity {
public:
	RemotePlayer() noexcept = default;
	~RemotePlayer() noexcept = default;

	__CLASS_NON_COPYABLE(RemotePlayer)

public:
	void initialize(const std::filesystem::path& file, u64 localId_) override;

	void update() override;

	void move_to(const std::chrono::system_clock::time_point& time, const Vector3& position) override;

private:
	/// <summary>
	/// 位置の補間計算
	/// </summary>
	void calculate_position();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

public:
	void set_target(Reference<IEntity> entity);

private:
	struct Waypoint {
		std::chrono::system_clock::duration timestamp;
		Vector3 position;
	};

	std::deque<Waypoint> waypoints;
	WorldTimer latency{};
	u32 waypointIndex{ 0 };
	std::chrono::system_clock::time_point startTime;
	std::chrono::system_clock::time_point fixedTime;

	const u32 interval = 2;
};
