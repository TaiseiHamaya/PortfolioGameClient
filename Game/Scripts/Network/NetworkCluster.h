#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "GameServer/GameServerConnectionManager.h"
#include "GameServer/GameServerPacketReceiver.h"
#include "GameServer/GameServerPacketSender.h"

class NetworkCluster {
public:
	NetworkCluster() = default;
	~NetworkCluster() = default;

	__CLASS_NON_COPYABLE(NetworkCluster)

public:
	void initialize();
	void setup();

	void finalize();

	void receive();
	void send();

public:
	Reference<GameServerPacketReceiver> get_receiver();
	Reference<GameServerConnectionManager> connection_manager();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	GameServerConnectionManager connectionManager;
	GameServerPacketReceiver packetReceiver;
	GameServerPacketSender packetSender;

	std::string msg;
};
