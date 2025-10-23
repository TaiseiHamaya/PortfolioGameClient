#pragma once

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Runtime/Input/InputHandler.h>

class Player;
class WorldInstance;
class ZoneHandler;

/// <summary>
/// インゲームの入力ハンドラ
/// </summary>
class GameInputHandler final {
public:
	GameInputHandler() = default;
	~GameInputHandler() = default;

	__CLASS_NON_COPYABLE(GameInputHandler)

public:
	void initialize();

	void setup(Reference<ZoneHandler> zoneHandler_);
	
	/// <summary>
	/// 入力処理
	/// </summary>
	void input();

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

private:
	InputHandler<KeyID> actionHandlerK;
	InputHandler<PadID> actionHandlerP;
	std::vector<std::pair<KeyID, PadID>> inputKey;

	Vector2 inputDirection;
	Vector2 xzDirection;

	Reference<ZoneHandler> zoneHandler;
	Reference<const WorldInstance> camera;
	
	Reference<Player> player;

#ifdef DEBUG_FEATURES_ENABLE
	WorldTimer timer;
#endif // DEBUG_FEATURES_ENABLE

public:
	void set_instances(Reference<Player> player_, Reference<const WorldInstance> camera_);
};
