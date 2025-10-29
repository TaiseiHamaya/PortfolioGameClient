#pragma once

#include <chrono>
#include <list>
#include <string>

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
		std::chrono::system_clock::time_point timestamp;
		Vector3 position;
	};

	std::list<Waypoint> waypoints;
};
