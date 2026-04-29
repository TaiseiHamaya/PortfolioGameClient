#pragma once

#include <functional>
#include <memory>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Game/Zone/Command/IZoneCommand.h"

class EntityManager;
class GameLogWindowManager;
class GameServerConnectionManager;
class Player;

/// <summary>
/// ログインメッセージのハンドラ
/// </summary>
class ZoneStartGameMessageHandler {
public:
	ZoneStartGameMessageHandler() = default;
	~ZoneStartGameMessageHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneStartGameMessageHandler)

public:
	using CommandStack = std::function<void(std::unique_ptr<IZoneCommand>)>;

	void setup(
		Reference<EntityManager> entityManager_,
		Reference<GameLogWindowManager> gameLogWindowManager_,
		Reference<GameServerConnectionManager> gameServerConnectionManager_,
		CommandStack commandStack_
	);

	void set_player(Reference<Player> player_);

	void operator()(const Proto::ToClientMessage& packet);

private:
	Reference<EntityManager> entityManager;
	Reference<GameLogWindowManager> gameLogWindowManager;
	Reference<GameServerConnectionManager> gameServerConnectionManager;
	Reference<Player> player;
	CommandStack commandStack;
};
