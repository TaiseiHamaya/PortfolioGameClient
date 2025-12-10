#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Runtime/Input/InputHandler.h>

namespace szg {

class WorldInstance;

} // namespace szg

class Player;
class ZoneHandler;

/// <summary>
/// インゲームの入力ハンドラ
/// </summary>
class GameInputHandler final : public szg::ISceneScript {
public:
	GameInputHandler() = default;
	~GameInputHandler() = default;

	SZG_CLASS_MOVE_ONLY(GameInputHandler)

public:
	void initialize();

	void setup(Reference<ZoneHandler> zoneHandler_);

	void prev_update() override;

	void post_update() override;

	/// <summary>
	/// 入力処理
	/// </summary>
	void input();

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

private:
	szg::InputHandler<szg::KeyID> actionHandlerK;
	szg::InputHandler<szg::PadID> actionHandlerP;
	std::vector<std::pair<szg::KeyID, szg::PadID>> inputKey;

	Vector2 inputDirection;
	Vector2 xzDirection;

	Reference<ZoneHandler> zoneHandler;
	Reference<const szg::WorldInstance> camera;

	Reference<Player> player;

#ifdef DEBUG_FEATURES_ENABLE
	static constexpr r32 BOT_MOVE_TIME{ 3.0f };
	szg::WorldTimer timer;
#endif // DEBUG_FEATURES_ENABLE

public:
	void set_instances(Reference<Player> player_, Reference<const szg::WorldInstance> camera_);
};
