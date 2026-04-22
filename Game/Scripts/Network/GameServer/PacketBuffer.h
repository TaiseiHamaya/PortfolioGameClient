#pragma once

#include <span>
#include <vector>

#include "process/gateway/packet.pb.h"

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// TCP Stream解決用バッファ
/// </summary>
class ReceiveBuffer final {
public:
	ReceiveBuffer() = default;
	~ReceiveBuffer() = default;

	SZG_CLASS_MOVE_ONLY(ReceiveBuffer)

public:
	/// <summary>
	/// パケット解析
	/// </summary>
	/// <param name="data"></param>
	/// <returns>解析したProto::ToClientMessage配列</returns>
	std::vector<Proto::ToClientMessage> resolve_packets(std::span<u8> data);

private:
	/// <summary>
	/// パケット内のLengthヘッダを取得
	/// </summary>
	/// <param name="data"></param>
	/// <returns>ヘッダ解析が完了している場合はtrue</returns>
	bool read_length_header(std::span<u8>& data);

private:
	u8 received_header_size{ 0 };
	u32 size{ 0 };
	std::vector<u8> buffer{};
};
