#pragma once

#include <span>
#include <vector>

#include "Scripts/Proto/types.pb.h"

#include <Library/Utility/Tools/ConstructorMacro.h>

class ReceiveBuffer final {
public:
	ReceiveBuffer() = default;
	~ReceiveBuffer() = default;

	__CLASS_NON_COPYABLE(ReceiveBuffer)

public:
	std::vector<Proto::Packet> resolve_packets(std::span<u8> data);

private:
	bool read_length_header(std::span<u8>& data);

private:
	u8 received_size{ 0 };
	u8 size{ 0 };
	std::vector<u8> buffer{};
};
