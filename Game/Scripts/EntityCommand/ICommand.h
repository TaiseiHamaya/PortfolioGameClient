#pragma once

#include "Scripts/IEntity/BaseEntity.h"

class ICommand {
public:
	ICommand() = default;
	virtual ~ICommand() = default;

	virtual void execute(Reference<BaseEntity>) = 0;
};
