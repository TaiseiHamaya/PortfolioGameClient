#pragma once

#include <string>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Scripts/Proto/types.pb.h>

#include <asio/error_code.hpp>

class GameServerConnectionManager;

/// <summary>
/// メッセージ送信用クラス
/// </summary>
class GameServerPacketSender final {
public:
	GameServerPacketSender() = default;
	~GameServerPacketSender() = default;

	__CLASS_NON_COPYABLE(GameServerPacketSender)

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

	/// <summary>
	/// スタック
	/// </summary>
	/// <param name="packet"></param>
	void stack_packet(const Proto::Packet& packet);
	
	/// <summary>
	/// 送信
	/// </summary>
	void send_all_packets();

private:
	void on_send_handler(const asio::error_code& errorCode, std::size_t bytesTransferred);

private:
	std::vector<std::vector<u8>> packetStack;
	Reference<GameServerConnectionManager> connectionManager;
};
