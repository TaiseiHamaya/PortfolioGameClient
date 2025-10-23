#pragma once

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Scripts/Proto/types.pb.h>

#include <Scripts/Game/Zone/ZoneCommand/Command/IZoneCommand.h>

class Player;
class EntityManager;
class GameServerConnectionManager;
class GameServerPacketReceiver;
class GameServerPacketSender;

/// <summary>
/// ゾーン処理に関するハンドラ
/// </summary>
class ZoneHandler final {
public:
	ZoneHandler() = default;
	~ZoneHandler() = default;

	__CLASS_NON_COPYABLE(ZoneHandler)

public:
	void setup(
		Reference<EntityManager> entityManager_,
		Reference<GameServerConnectionManager> gameServerConnectionManager_,
		Reference<GameServerPacketReceiver> gameServerPacketReceiver_,
		Reference<GameServerPacketSender> gameServerPacketSender_
	);

	/// <summary>
	/// コマンドの実行
	/// </summary>
	void execute_commands();

	/// <summary>
	/// ハンドリング
	/// </summary>
	void handle_zone();

	/// <summary>
	/// コマンドのスタック
	/// </summary>
	/// <param name="command"></param>
	void stack_command(std::unique_ptr<IZoneCommand> command);

	/// <summary>
	/// プレイヤーを移動させる
	/// </summary>
	/// <param name="position">位置</param>
	void move_client_player(const Vector3& position);

private:
	void process_text_message(Proto::TextMessageType type, const std::string& payload);
	void process_login_packet(Proto::LoginPacketType type, const std::string& payload);
	void process_logout_packet(Proto::LogoutPacketType type, const std::string& payload);
	void process_sync_packet(Proto::SyncPacketType type, const std::string& payload);

private:
	std::vector<std::unique_ptr<IZoneCommand>> zoneCommands;

	Reference<Player> player;
	Reference<EntityManager> entityManager;
	Reference<GameServerConnectionManager> gameServerConnectionManager;
	Reference<GameServerPacketSender> gameServerPacketSender;
	Reference<GameServerPacketReceiver> gameServerPacketReceiver;

public:
	void set_player(Reference<Player> player_);
};
