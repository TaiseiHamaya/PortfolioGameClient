#pragma once

#include <span>
#include <vector>

#include "Scripts/Proto/types.pb.h"

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// TCP Stream解決用バッファ
/// </summary>
class ReceiveBuffer final {
public:
	ReceiveBuffer() = default;
	~ReceiveBuffer() = default;

	__CLASS_NON_COPYABLE(ReceiveBuffer)

public:
	/// <summary>
	/// パケット解析
	/// </summary>
	/// <param name="data"></param>
	/// <returns>解析したProto::Packet配列</returns>
	std::vector<Proto::Packet> resolve_packets(std::span<u8> data);

private:
	/// <summary>
	/// パケット内のLengthヘッダを取得
	/// </summary>
	/// <param name="data"></param>
	/// <returns>ヘッダ解析が完了している場合はtrue</returns>
	bool read_length_header(std::span<u8>& data);

private:
	u8 received_size{ 0 };
	u8 size{ 0 };
	std::vector<u8> buffer{};
};
