#pragma once

#include "../ICommand.h"

class StartActionCommand final : public ICommand {
public:
	StartActionCommand() = default;
	~StartActionCommand() = default;

	void execute(Reference<BaseEntity> entity);

private:
	u32 actionIndex;
};
