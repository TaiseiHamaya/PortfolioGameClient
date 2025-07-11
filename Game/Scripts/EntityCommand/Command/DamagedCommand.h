#pragma once

#include "../ICommand.h"

class DamagedCommand final : public ICommand {
public:
	DamagedCommand(i32 damage_);
	~DamagedCommand() = default;

	void execute(Reference<IEntity> entity);
	
private:
	i32 damage;
};
