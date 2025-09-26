#include "GameServerPacketReceiver.h"

#include "GameServerConnectionManager.h"

#include <Engine/Application/Output.h>

void GameServerPacketReceiver::initialize() {
}

void GameServerPacketReceiver::setup(Reference<GameServerConnectionManager> connectionManager_) {
	connectionManager = connectionManager_;
}

void GameServerPacketReceiver::finalize() {
	packetStack.clear();
}

const std::vector<Proto::Packet>& GameServerPacketReceiver::get_packet_stack() {
	return packetStack;
}

void GameServerPacketReceiver::poll_packets() {
	if (!connectionManager || !connectionManager->is_connected()) {
		return;
	}
	asio::ip::tcp::socket& socket = connectionManager->get_socket();

	//socket.async_read_some()
}

void GameServerPacketReceiver::on_receive_handler() {
}
