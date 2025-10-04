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
	std::vector<u8> serialized;
	u32 size = static_cast<u32>(packet.ByteSizeLong());
	serialized.resize(size);
	packet.SerializeToArray(serialized.data(), size);
	std::vector<u8> sizePacket;
	for (i32 i = 3; i >= 0; --i) {
		sizePacket.emplace_back((size >> i * 8) & 0xff);
	}
	packetStack.emplace_back(std::move(sizePacket));
	packetStack.emplace_back(std::move(serialized));
}

void GameServerPacketSender::send_all_packets() {
	if (!connectionManager || !connectionManager->is_connected()) {
		return;
	}
	asio::ip::tcp::socket& socket = connectionManager->get_socket();
	for (auto& packet : packetStack) {
		asio::mutable_buffer buffer = asio::buffer(packet);
		asio::async_write(socket, buffer, [&, packet = std::move(packet)](asio::error_code errorCode, std::size_t bytesTransferred) {
			on_send_handler(errorCode, bytesTransferred);
		});
	}
	packetStack.clear();
}

void GameServerPacketSender::on_send_handler(const asio::error_code& errorCode, std::size_t bytesTransferred) {
	if (errorCode) {
		Warning("Failed to send packet to game server: {}\n", errorCode.message());
	}
	else {
		Information("Sent {} bytes to game server", bytesTransferred);
	}
}
