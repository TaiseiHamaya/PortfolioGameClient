#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Scripts/Proto/types.pb.h>

#include <Scripts/Game/Zone/Command/IZoneCommand.h>

class Player;
class EntityManager;
class EnemyManager;
class GameServerConnectionManager;
class GameServerPacketReceiver;
class GameServerPacketSender;

/// <summary>
/// ゾーン処理に関するハンドラ
/// </summary>
class ZoneHandler final : public ISceneScript {
public:
	ZoneHandler() = default;
	~ZoneHandler() = default;

	__CLASS_NON_COPYABLE(ZoneHandler)

public:
	void setup(
		Reference<EntityManager> entityManager_,
		Reference<EnemyManager> enemyManager_,
		Reference<GameServerConnectionManager> gameServerConnectionManager_,
		Reference<GameServerPacketReceiver> gameServerPacketReceiver_,
		Reference<GameServerPacketSender> gameServerPacketSender_
	);

	void prev_update() override;

	void post_update() override;

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

	void request_play_action(u32 actionId);

private:
	void process_text_message(Proto::CategoryTextMessage type, const std::string& payload);
	void process_login_message(Proto::CategoryLoginMessage type, const std::string& payload);
	void process_logout_message(Proto::CategoryLogoutMessage type, const std::string& payload);
	void process_sync_message(Proto::CategorySyncMessage type, const std::string& payload);
	void process_entity_message(Proto::CategoryEnemyMessage category, const std::string& payload);

private:
	std::vector<std::unique_ptr<IZoneCommand>> zoneCommands;

	Reference<Player> player;
	Reference<EntityManager> entityManager;
	Reference<EnemyManager> enemyManager;
	Reference<GameServerConnectionManager> gameServerConnectionManager;
	Reference<GameServerPacketSender> gameServerPacketSender;
	Reference<GameServerPacketReceiver> gameServerPacketReceiver;

public:
	void set_player(Reference<Player> player_);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();

private:
	i32 debugCommandCount{ 0 };
	i32 debugRecivedMessageCount{ 0 };
	i32 debugSendedMessageCount{ 0 };
#endif // DEBUG_FEATURES_ENABLE
};
