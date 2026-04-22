#pragma once

#include <Library/Utility/Template/SingletonInterface.h>

#include "GameServer/GameServerConnectionManager.h"
#include "GameServer/GameServerPacketReceiver.h"
#include "GameServer/GameServerPacketSender.h"

class NetworkCluster final : public SingletonInterface<NetworkCluster> {
	SZG_CLASS_SINGLETON(NetworkCluster)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 遅延初期化
	/// </summary>
	static void Setup(const std::string& addr, u16 port);

	static void Connect();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// メッセージ受け取り
	/// </summary>
	static void Receive();

	/// <summary>
	/// メッセージ送信
	/// </summary>
	static void Send();

private:
	GameServerConnectionManager connectionManager;
	GameServerPacketReceiver packetReceiver;
	GameServerPacketSender packetSender;

public:
	static Reference<GameServerConnectionManager> ConnectionManagerMut();
	static Reference<GameServerPacketReceiver> ReceiverMut();
	static Reference<GameServerPacketSender> SenderMut();
};
