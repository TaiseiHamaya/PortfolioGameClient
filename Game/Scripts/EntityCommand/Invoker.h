#pragma once

#include <list>

#include "Scripts/IEntity/BaseEntity.h"
#include "Scripts/EntityCommand/ICommand.h"

class Invoker {

public:
	void run();

public:
	void register_entry(Reference<BaseEntity> receiver, u64 commandIndex);

private:
	std::vector<std::unique_ptr<ICommand>> commands;
	std::list<std::function<void(void)>> commandEntries;
};
