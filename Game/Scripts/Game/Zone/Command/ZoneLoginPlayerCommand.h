#pragma once

#include "./IZoneCommand.h"

#include <string>

#include <Library/Math/Vector3.h>

/// <summary>
/// ログイン処理
/// </summary>
class ZoneLoginPlayerCommand final : public IZoneCommand {
public:
	ZoneLoginPlayerCommand(u64 serverId_, const std::string& name_, const Vector3& position_);

public:
	void execute(Zone& zone) override;

private:
	u64 serverId;
	std::string name;
	Vector3 position;
};
