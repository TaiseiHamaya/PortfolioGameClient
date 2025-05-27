#pragma once

#include "../ICommand.h"

class DamagedCommand final : public ICommand {
public:
	DamagedCommand() = default;
	~DamagedCommand() = default;

	void execute(Reference<IEntity> entity);
	
private:
	i32 damage;
};
