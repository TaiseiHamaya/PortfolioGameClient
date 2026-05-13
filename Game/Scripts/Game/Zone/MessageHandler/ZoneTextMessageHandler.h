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
/// テキストメッセージのハンドラ
/// </summary>
class ZoneTextMessageHandler {
public:
	ZoneTextMessageHandler() = default;
	~ZoneTextMessageHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneTextMessageHandler)

public:
	void setup(
		Reference<EntityManager> entityManager_,
		Reference<GameLogWindowManager> gameLogWindowManager_
	);

	void operator()(const Proto::ToClientMessage& packet);

private:
	Reference<EntityManager> entityManager;
	Reference<GameLogWindowManager> gameLogWindowManager;
};
