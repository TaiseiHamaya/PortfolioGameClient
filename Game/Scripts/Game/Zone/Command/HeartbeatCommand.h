#pragma once

#include "./IZoneCommand.h"

#include <Library/Utility/Template/Reference.h>

#include "Scripts/Network/Heartbeat.h"

/// <summary>
/// ハートビート受け取り時に発行されるコマンド
/// </summary>
class HeartbeatCommand final : public IZoneCommand {
public:
	HeartbeatCommand(i64 timestamp);

public:
	void execute(Zone& zone) override;

private:
	i64 timestamp;

public:
	static void InitializeHeartbeat(Reference<Heartbeat> heartbeat);

private:
	static inline Reference<Heartbeat> heartbeat{};
};
