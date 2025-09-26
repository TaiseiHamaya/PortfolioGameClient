#pragma once

#include <vector>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <asio/error_code.hpp>

#include <Scripts/Proto/types.pb.h>

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
	const std::vector<Proto::Packet>& get_packet_stack();
	void poll_packets();

private:
	void on_receive_handler();

private:
	std::vector<Proto::Packet> packetStack;
	Reference<GameServerConnectionManager> connectionManager;
};
