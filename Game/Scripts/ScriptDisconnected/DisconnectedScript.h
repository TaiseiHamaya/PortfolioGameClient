#pragma once

#include <Engine/Runtime/SceneScript/ISceneScript.h>

#include <Engine/Runtime/Clock/WorldTimer.h>

class DisconnectedScript final : public szg::ISceneScript {
public:
	DisconnectedScript() = default;
	~DisconnectedScript() noexcept = default;

	SZG_CLASS_MOVE_ONLY(DisconnectedScript)

public:
	void prev_update() override;

private:
	szg::WorldTimer timer{ 5.0f };
};
