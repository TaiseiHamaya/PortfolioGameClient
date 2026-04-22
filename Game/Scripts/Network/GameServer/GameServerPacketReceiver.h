#pragma once

#include <vector>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "PacketBuffer.h"

class GameServerConnectionManager;

/// <summary>
/// メッセージ受け取り用クラス
/// </summary>
class GameServerPacketReceiver final {
public:
	GameServerPacketReceiver() = default;
	~GameServerPacketReceiver() = default;

	SZG_CLASS_MOVE_ONLY(GameServerPacketReceiver)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize();

	/// <summary>
	/// 遅延初期化
	/// </summary>
	/// <param name="connectionManager_"></param>
	void setup(Reference<GameServerConnectionManager> connectionManager_);

	/// <summary>
	/// 終了処理
	/// </summary>
	void finalize();

public:
	/// <summary>
	/// Recv済みパケットの転送
	/// </summary>
	/// <returns></returns>
	std::vector<Proto::ToClientMessage> take_packet_stack();

	/// <summary>
	/// メッセージ読み取り
	/// </summary>
	void read_packets();

private:
	static constexpr size_t BufferSize = 4096;

	std::vector<Proto::ToClientMessage> packetStack;
	Reference<GameServerConnectionManager> connectionManager;

	ReceiveBuffer receiveBuffer;
};
