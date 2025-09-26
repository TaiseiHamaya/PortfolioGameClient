#include "GameServerPacketSender.h"

#include "GameServerConnectionManager.h"

#include <Engine/Application/Output.h>

void GameServerPacketSender::initialize() {
}

void GameServerPacketSender::setup(Reference<GameServerConnectionManager> connectionManager_) {
	connectionManager = connectionManager_;
}

void GameServerPacketSender::finalize() {
	packetStack.clear();
}

void GameServerPacketSender::stack_packet(const Proto::Packet& packet) {
	packetStack.emplace_back(packet.SerializeAsString());
}

void GameServerPacketSender::send_all_packets() {
	if (!connectionManager || !connectionManager->is_connected()) {
		return;
	}
	asio::ip::tcp::socket& socket = connectionManager->get_socket();
	for (auto& packet : packetStack) {
		asio::async_write(socket, asio::buffer(packet), [&](asio::error_code errorCode, std::size_t bytesTransferred) {
			on_send_handler(errorCode, bytesTransferred);
		});
	}
	packetStack.clear();
}

void GameServerPacketSender::on_send_handler(const asio::error_code& errorCode, std::size_t bytesTransferred) {
}
