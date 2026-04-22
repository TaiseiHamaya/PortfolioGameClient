#pragma once

#include <functional>
#include <memory>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Game/Zone/Command/IZoneCommand.h"

class EntityManager;
class GameLogWindowManager;

/// <summary>
/// ログアウトメッセージのハンドラ
/// </summary>
class ZoneLogoutMessageHandler {
public:
	ZoneLogoutMessageHandler() = default;
	~ZoneLogoutMessageHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneLogoutMessageHandler)

public:
	using CommandStack = std::function<void(std::unique_ptr<IZoneCommand>)>;

	void setup(
		Reference<EntityManager> entityManager_,
		Reference<GameLogWindowManager> gameLogWindowManager_,
		CommandStack commandStack_
	);

	void operator()(const Proto::ToClientMessage& packet);

private:
	Reference<EntityManager> entityManager;
	Reference<GameLogWindowManager> gameLogWindowManager;
	CommandStack commandStack;
};
