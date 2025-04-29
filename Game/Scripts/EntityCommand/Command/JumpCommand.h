#pragma once

#include "../ICommand.h"

class JumpCommand final : public ICommand {
public:
	JumpCommand() = default;
	~JumpCommand() = default;

	void execute(Reference<BaseEntity> entity);
};
