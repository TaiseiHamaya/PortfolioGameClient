#pragma once

#include <Engine/Runtime/Scene/Scene.h>

class DisconnectedScene final : public szg::Scene {
public:
	DisconnectedScene() noexcept;
	~DisconnectedScene() noexcept override = default;

	SZG_CLASS_MOVE_ONLY(DisconnectedScene)

public:
	void custom_setup() override;

private:
};
