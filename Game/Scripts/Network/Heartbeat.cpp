#include "Heartbeat.h"

#include <Engine/Application/Logger.h>
#include <Engine/Runtime/Clock/WorldClock.h>

#include "./NetworkCluster.h"

void Heartbeat::update() {
	timer.back();

	if (timer < 0.0f && !latestSendHeartbeatTimestamp) {
		heartbeat();
		timer.set(1.0f);
	}

	if (latestReceivedHeartbeatTimestamp && latestSendHeartbeatTimestamp) {
		auto duration = *latestReceivedHeartbeatTimestamp - *latestSendHeartbeatTimestamp;
		szgInformation("Round trip time: {} ms", duration);

		const std::chrono::microseconds threshold = std::chrono::seconds(10);
		if (duration > threshold) {
			// 10秒以上前のハートビートレスポンスしか受け取っていない場合は切断とみなす
			NetworkCluster::GetInstance().ConnectionManagerMut()->disconnect();
			latestReceivedHeartbeatTimestamp.reset();
			latestSendHeartbeatTimestamp.reset();
		}
	}
	else if (!latestReceivedHeartbeatTimestamp && latestSendHeartbeatTimestamp) {
		auto duration = szg::WorldClock::FrameSysTime() - *latestSendHeartbeatTimestamp;

		szgInformation("Waiting for heartbeat response: {} ms", duration);

		const std::chrono::microseconds threshold = std::chrono::seconds(10);
		if (duration > threshold) {
			// 10秒以上前のハートビートしか送信していない場合は切断とみなす
			NetworkCluster::GetInstance().ConnectionManagerMut()->disconnect();
			szgInformation("Heartbeat timeout: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
			latestSendHeartbeatTimestamp.reset();
		}
	}
}

void Heartbeat::on_heartbeat_response(i64 timestamp_micro) {
	latestReceivedHeartbeatTimestamp = std::chrono::system_clock::time_point(std::chrono::microseconds(timestamp_micro));
}

void Heartbeat::heartbeat() {
	NetworkCluster& cluster = NetworkCluster::GetInstance();
	if (!cluster.ConnectionManagerMut()->is_connected()) {
		return;
	}

	Proto::ToServerMessage message;
	Proto::PayloadHeartbeatRequest* payload = message.mutable_heartbeat_request();
	std::chrono::system_clock::time_point frameTime = szg::WorldClock::FrameSysTime();
	latestSendHeartbeatTimestamp = frameTime;
	payload->set_timestamp(std::chrono::duration_cast<std::chrono::microseconds>(frameTime.time_since_epoch()).count());
	cluster.SenderMut()->stack_packet(message);
}
