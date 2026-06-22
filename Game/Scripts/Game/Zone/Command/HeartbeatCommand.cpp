#include "HeartbeatCommand.h"

HeartbeatCommand::HeartbeatCommand(i64 timestamp_) :
	timestamp(timestamp_) {
}

void HeartbeatCommand::execute(Zone& zone) {
	if (!heartbeat) {
		return;
	}

	heartbeat->on_heartbeat_response(timestamp);
}

void HeartbeatCommand::InitializeHeartbeat(Reference<Heartbeat> heartbeat_) {
	heartbeat = heartbeat_;
}
