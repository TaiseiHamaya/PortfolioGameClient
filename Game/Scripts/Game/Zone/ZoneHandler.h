#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Game/Zone/Command/IZoneCommand.h"
#include "Scripts/Game/Zone/MessageHandler/ZoneEnemyMessageHandler.h"
#include "Scripts/Game/Zone/MessageHandler/ZoneStartGameMessageHandler.h"
#include "Scripts/Game/Zone/MessageHandler/ZoneNotificationMessageHandler.h"
#include "Scripts/Game/Zone/MessageHandler/ZoneSyncMessageHandler.h"
#include "Scripts/Game/Zone/MessageHandler/ZoneTextMessageHandler.h"
#include "Scripts/Game/Zone/Zone.h"
#include "Scripts/Manager/ChatBoxManager.h"
#include "Scripts/Network/GameServer/ReceivedMessageRouter.h"

class Player;
class EntityManager;
class EnemyManager;
class GameServerConnectionManager;
class GameServerPacketReceiver;
class GameServerPacketSender;
class GameLogWindowManager;

/// <summary>
/// ゾーン処理に関するハンドラ
/// </summary>
class ZoneHandler final : public szg::ISceneScript {
public:
	ZoneHandler() = default;
	~ZoneHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneHandler)

public:
	void setup(
		Reference<EntityManager> entityManager_,
		Reference<EnemyManager> enemyManager_,
		Reference<GameLogWindowManager> gameLogWindowManager_
	);

	void prev_update() override;

	void post_update() override;

	void finalize() override;

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

public:
	Reference<const ChatBoxManager> chat_box_imm() const noexcept;

private:
	std::vector<std::unique_ptr<IZoneCommand>> zoneCommands;

	Reference<Player> player;
	Reference<EntityManager> entityManager;
	Reference<EnemyManager> enemyManager;
	Reference<GameServerConnectionManager> gameServerConnectionManager;
	Reference<GameServerPacketSender> gameServerPacketSender;
	Reference<GameServerPacketReceiver> gameServerPacketReceiver;
	Reference<GameLogWindowManager> gameLogWindowManager;

	ChatBoxManager chatBoxManager;

	Zone zone;
	ReceivedMessageRouter router;

	ZoneTextMessageHandler textMessageHandler;
	ZoneStartGameMessageHandler startGameMessageHandler;
	ZoneNotificationMessageHandler notificationMessageHandler;
	ZoneSyncMessageHandler syncMessageHandler;
	ZoneEnemyMessageHandler enemyMessageHandler;

public:
	void set_player(Reference<Player> player_);
	void set_effect_manager(Reference<EffectManager> effectManager_);
	void set_camera_instance(Reference<const szg::WorldInstance> cameraInstance_);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();

private:
	i32 debugCommandCount{ 0 };
	i32 debugReceivedMessageCount{ 0 };
	i32 debugSentMessageCount{ 0 };

	std::string msg;
#endif // DEBUG_FEATURES_ENABLE
};
