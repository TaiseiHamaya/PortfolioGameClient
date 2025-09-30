#pragma once

#include <vector>

#include <Scripts/Proto/types.pb.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "PacketBuffer.h"

class GameServerConnectionManager;

class GameServerPacketReceiver final {
public:
	GameServerPacketReceiver() = default;
	~GameServerPacketReceiver() = default;

	__CLASS_NON_COPYABLE(GameServerPacketReceiver)

public:
	void initialize();
	void setup(Reference<GameServerConnectionManager> connectionManager_);
	void finalize();

public:
	std::vector<Proto::Packet> take_packet_stack();
	void read_packets();

private:
	static constexpr size_t BufferSize = 4096;

	std::vector<Proto::Packet> packetStack;
	Reference<GameServerConnectionManager> connectionManager;

	ReceiveBuffer receiveBuffer;
};
