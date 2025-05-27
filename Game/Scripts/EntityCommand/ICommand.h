#pragma once

#include "Scripts/IEntity/IEntity.h"

class ICommand {
public:
	ICommand() = default;
	virtual ~ICommand() = default;

	virtual void execute(Reference<IEntity>) = 0;
};
