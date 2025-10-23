#pragma once

#include "./IZoneCommand.h"

#include <chrono>

#include <Library/Math/Quaternion.h>
#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>

class IEntity;

/// <summary>
/// 位置同期処理
/// </summary>
class ZoneSyncTransformCommand final : public IZoneCommand {
public:
	ZoneSyncTransformCommand(Reference<IEntity> entity, const std::chrono::steady_clock::time_point& time, const Vector3& position_);

public:
	void execute() override;

private:
	Reference<IEntity> entity;
	std::chrono::steady_clock::time_point time;
	Vector3 position;
};
