#include "GameServerPacketReceiver.h"

#include "GameServerConnectionManager.h"

void GameServerPacketReceiver::initialize() {
}

void GameServerPacketReceiver::setup(Reference<GameServerConnectionManager> connectionManager_) {
	connectionManager = connectionManager_;
}

void GameServerPacketReceiver::finalize() {
	packetStack.clear();
}

std::vector<Proto::Packet> GameServerPacketReceiver::take_packet_stack() {
	return std::move(packetStack);
}

void GameServerPacketReceiver::read_packets() {
	if (!connectionManager || !connectionManager->is_established()) {
		return;
	}

	auto& socket = connectionManager->get_socket();

	if (socket.available() == 0) {
		return;
	}

	std::array<u8, BufferSize> buffer{};
	size_t bytesRead = socket.read_some(asio::buffer(buffer));
	std::span<u8> dataSpan{ buffer.data(), bytesRead };
	auto packets = receiveBuffer.resolve_packets(dataSpan);
	packetStack.insert(packetStack.end(), packets.begin(), packets.end());
}
