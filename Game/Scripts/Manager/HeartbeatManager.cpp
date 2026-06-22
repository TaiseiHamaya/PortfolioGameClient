#include "HeartbeatManager.h"

void HeartbeatManager::prev_update() {
	heartbeat.update();
}

Reference<Heartbeat> HeartbeatManager::heartbeat_mut() {
	return heartbeat;
}
