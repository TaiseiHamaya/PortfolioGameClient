#pragma once

#include <Engine/Runtime/Scene/Scene.h>

class LoginScene final : public szg::Scene {
public:
	LoginScene() noexcept;
	~LoginScene() noexcept override = default;

	SZG_CLASS_MOVE_ONLY(LoginScene)

public:
	void custom_setup() override;

private:
};
