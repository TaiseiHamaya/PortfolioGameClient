#include "PacketBuffer.h"

#include <Engine/Application/Logger.h>

std::vector<Proto::Packet> ReceiveBuffer::resolve_packets(std::span<u8> data) {
	std::vector<Proto::Packet> result;

	while (!data.empty()) {
		// ヘッダー読み込み
		if (!read_length_header(data)) {
			// ヘッダーが読み込まれていない場合、処理終了
			break;
		}
		if (size == 0) {
			return result;
		}
		// ペイロード読み込み
		u64 readSize = std::min<u64>(static_cast<u64>(size), data.size());
		for (u64 i = 0; i < readSize; ++i) {
			buffer.emplace_back(data[i]);
			--size;
		}
		data = data.subspan(readSize);

		if (size == 0) {
			// パケットが完成した場合、デコードして結果に追加
			Proto::Packet packet;
			if (packet.ParseFromArray(buffer.data(), static_cast<int>(buffer.size()))) {
				result.emplace_back(std::move(packet));
			}
			else {
				szgError("Failed to parse packet from buffer.");
			}
			// 状態リセット
			received_size = 0;
			size = 0;
			buffer.clear();
		}
	}

	return result;
}

bool ReceiveBuffer::read_length_header(std::span<u8>& data) {
	constexpr u8 sizeof32 = sizeof(u32);
	constexpr u8 sizeof8 = sizeof(u8);

	// ヘッダーが読み込まれている状態
	if (received_size >= sizeof32) {
		return true;
	}

	u8 neededSize = sizeof32 - received_size;
	u64 readSize = std::min<u64>(static_cast<u64>(neededSize), data.size());
	for (u64 i = 0; i < readSize; ++i) {
		size |= static_cast<u32>(data[i]) << (received_size * 8);
		++received_size;
	}
	data = data.subspan(readSize);

	buffer.reserve(size);

	return received_size >= sizeof32;
}
