#pragma once

#include <Engine/Runtime/SceneScript/ISceneScript.h>

#include <Library/Utility/Template/Reference.h>

#include "Scripts/Network/Heartbeat.h"

/// <summary>
/// ハートビートマネージャ
/// ハートビート機構の管理とシーン統合を行う
/// </summary>
class HeartbeatManager : public szg::ISceneScript {
public:
	void prev_update() override;

public:
	Reference<Heartbeat> heartbeat_mut();

private:
	Heartbeat heartbeat;
};
