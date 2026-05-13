#pragma once

#include "./IZoneCommand.h"

/// <summary>
/// ログアウト処理
/// </summary>
class ZoneLogoutPlayerCommand final : public IZoneCommand {
public:
	ZoneLogoutPlayerCommand(u64 serverId_);

public:
	void execute(Zone& zone) override;

private:
	u64 serverId;
};
