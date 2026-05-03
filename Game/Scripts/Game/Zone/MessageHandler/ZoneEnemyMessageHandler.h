#pragma once

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Game/Zone/Command/IZoneCommand.h"

/// <summary>
/// 敵メッセージのハンドラ
/// </summary>
class ZoneEnemyMessageHandler {
public:
	ZoneEnemyMessageHandler() = default;
	~ZoneEnemyMessageHandler() = default;

	SZG_CLASS_MOVE_ONLY(ZoneEnemyMessageHandler)

public:
	void setup(
		std::function<void(std::unique_ptr<IZoneCommand>)> commandStack_
	);

	void operator()(const Proto::ToClientMessage& packet);

private:
	std::function<void(std::unique_ptr<IZoneCommand>)> commandStack;
};
