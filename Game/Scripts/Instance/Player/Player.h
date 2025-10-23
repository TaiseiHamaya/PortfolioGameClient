#pragma once

#include <string>

#include "Scripts/Instance/IEntity/IEntity.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player final : public IEntity {
public:
	void initialize(const std::filesystem::path& file) override;

	void move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) override;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

public:
	void set_target(Reference<IEntity> entity);
};
