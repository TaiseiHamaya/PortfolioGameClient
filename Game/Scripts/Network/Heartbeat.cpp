#include "Heartbeat.h"

#include <Engine/Runtime/Clock/WorldClock.h>

#include "./NetworkCluster.h"

void Heartbeat::update() {
	timer.back();

	if (timer < 0.0f) {
		heartbeat();
		timer.set(1.0f);
	}

	if (latestHeartbeatTimestamp) {
		i64 now = szg::WorldClock::FrameSysTime().time_since_epoch().count();
		if (now - *latestHeartbeatTimestamp > 5000) {
			// 5秒以上前のハートビートレスポンスしか受け取っていない場合は切断とみなす
			NetworkCluster::GetInstance().ConnectionManagerMut()->disconnect();
			latestHeartbeatTimestamp.reset();
		}
	}
}

void Heartbeat::on_heartbeat_response(i64 timestamp) {
	latestHeartbeatTimestamp = timestamp;
}

void Heartbeat::heartbeat() {
	NetworkCluster& cluster = NetworkCluster::GetInstance();
	if (!cluster.ConnectionManagerMut()->is_connected()) {
		return;
	}

	Proto::ToServerMessage message;
	Proto::PayloadHeartbeatRequest* payload = message.mutable_heartbeat_request();
	payload->set_timestamp(
		szg::WorldClock::FrameSysTime().time_since_epoch().count()
	);
	cluster.SenderMut()->stack_packet(message);
}
