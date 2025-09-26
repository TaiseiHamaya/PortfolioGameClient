#pragma once

#include <vector>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <asio/error_code.hpp>

#include <Scripts/Proto/types.pb.h>

class GameServerConnectionManager;

class GameServerPacketSender final {
public:
	GameServerPacketSender() = default;
	~GameServerPacketSender() = default;

	__CLASS_NON_COPYABLE(GameServerPacketSender)

public:
	void initialize();
	void setup(Reference<GameServerConnectionManager> connectionManager_);
	void finalize();

	void stack_packet(const Proto::Packet& packet);
	void send_all_packets();

private:
	void on_send_handler(const asio::error_code& errorCode, std::size_t bytesTransferred);

private:
	std::vector<std::string> packetStack;
	Reference<GameServerConnectionManager> connectionManager;
};
