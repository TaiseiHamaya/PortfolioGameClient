#pragma once

#include <Engine/Runtime/SceneScript/ISceneScript.h>

#include <Library/Utility/Template/Reference.h>

#include "Scripts/Network/Heartbeat.h"

class HeartbeatManager : public szg::ISceneScript {
public:
	void prev_update() override;

public:
	Reference<Heartbeat> heartbeat_mut();

private:
	Heartbeat heartbeat;
};
