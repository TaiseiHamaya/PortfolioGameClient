#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "GameServer/GameServerConnectionManager.h"
#include "GameServer/GameServerPacketReceiver.h"
#include "GameServer/GameServerPacketSender.h"

class IEntity;

class NetworkCluster {
public:
	NetworkCluster() = default;
	~NetworkCluster() = default;

	// ConnectionManagerがコピー/ムーブ禁止のため
	__CLASS_NON_COPYMOVEABLE(NetworkCluster)

public:
	void initialize();
	void setup();

	void finalize();

	void receive();
	void send();

private:
	GameServerConnectionManager connectionManager;
	GameServerPacketReceiver packetReceiver;
	GameServerPacketSender packetSender;

	Reference<IEntity> player;

public:
	void set_player(Reference<IEntity> player_);
	Reference<GameServerPacketReceiver> get_receiver();
	Reference<GameServerConnectionManager> connection_manager();
	Reference<GameServerPacketSender> get_sender();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();

private:
	std::string msg;
#endif // DEBUG_FEATURES_ENABLE
};
