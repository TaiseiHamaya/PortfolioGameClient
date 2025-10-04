#pragma once

#include <chrono>
#include <list>
#include <string>

#include "Scripts/Instance/IEntity/IEntity.h"

class RemotePlayer final : public IEntity {
public:
	void initialize(const std::filesystem::path& file) override;

	void update() override;

	void move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) override;

private:
	void calculate_position();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

public:
	void set_target(Reference<IEntity> entity);

private:
	struct Waypoint {
		std::chrono::steady_clock::time_point timestamp;
		Vector3 position;
	};

	std::list<Waypoint> waypoints;
};
