#pragma once

#include <Engine/Runtime/Input/InputHandler.h>
#include <Library/Utility/Template/Reference.h>

#include "Scripts/IEntity/BaseEntity.h"
#include "Scripts/EntityCommand/Hander/IEntityCommandHandler.h"

class LocalPlayerCommandHandler final : public IEntityCommandHandler {
public:
	struct ActionKeys {
		KeyID key;
		PadID pad;
	};

public:
	void start(Reference<WorldInstance> camera_);
	void begin() override;
	void update() override;

private:
	const u32 NumPreAction{ 1 };
	std::optional<u32> actionBuffer; // 先行入力保存用

	Vector2 inputDirection;
	Vector2 xzDirection;
	InputHandler<KeyID> actionHandlerK;
	InputHandler<PadID> actionHandlerP;

	std::vector<ActionKeys> actionTriggers;

	Reference<WorldInstance> camera;
};
