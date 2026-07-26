#pragma once

#include <chrono>
#include <mutex>
#include <optional>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Runtime/Clock/WorldTimer.h>

/// <summary>
/// ハートビート通信管理
/// サーバーとのハートビート送受信をで接続状態を監視
/// </summary>
class Heartbeat {
public:
	Heartbeat() = default;
	~Heartbeat() = default;

	SZG_CLASS_DELETED(Heartbeat)

public:
	void update();

	void on_heartbeat_response(i64 timestamp);

private:
	void heartbeat();

private:
	szg::WorldTimer timer;

	std::mutex mutex;
	std::optional<std::chrono::system_clock::time_point> latestReceivedHeartbeatTimestamp;
	std::optional<std::chrono::system_clock::time_point> latestSendHeartbeatTimestamp;
};
