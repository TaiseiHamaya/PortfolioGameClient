#include "GameServerPacketSender.h"

#include "GameServerConnectionManager.h"

#include <Engine/Application/Logger.h>

void GameServerPacketSender::initialize() {
}

void GameServerPacketSender::setup(Reference<GameServerConnectionManager> connectionManager_) {
	connectionManager = connectionManager_;
}

void GameServerPacketSender::finalize() {
	packetStack.clear();
}

void GameServerPacketSender::stack_packet(const Proto::ToServerMessage& packet) {
	std::vector<u8> serialized;
	u32 size = static_cast<u32>(packet.ByteSizeLong());
	serialized.resize(size); // メモリ確保
	packet.SerializeToArray(serialized.data(), size); // シリアライズして書き込み
	// ヘッダー生成
	std::vector<u8> sizePacket;
	for (i32 i = 3; i >= 0; --i) {
		sizePacket.emplace_back((size >> i * 8) & 0xff);
	}
	// スタックに積む
	packetStack.emplace_back(std::move(sizePacket));
	packetStack.emplace_back(std::move(serialized));
}

void GameServerPacketSender::send_all_packets() {
	if (!connectionManager || !connectionManager->is_established()) {
		return;
	}
	asio::ip::tcp::socket& socket = connectionManager->get_socket();
	for (auto& packet : packetStack) {
		// Moveしてラムダに転送することで、送信完了までデータの寿命を保証する
		asio::mutable_buffer buffer = asio::buffer(packet);
		asio::async_write(socket, buffer, [&, packet = std::move(packet)](asio::error_code errorCode, std::size_t bytesTransferred) {
			on_send_handler(errorCode, bytesTransferred);
		});
	}
	packetStack.clear();
}

u64 GameServerPacketSender::packet_count() const noexcept {
	return packetStack.size();
}

void GameServerPacketSender::on_send_handler(const asio::error_code& errorCode, std::size_t bytesTransferred) {
	if (errorCode) {
		szgWarning("Failed to send packet to game server: {}\n", errorCode.message());
	}
	else {
		szgInformation("Sent {} bytes to game server", bytesTransferred);
	}
}
