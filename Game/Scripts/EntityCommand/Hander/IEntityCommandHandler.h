#pragma once

#include <list>
#include <vector>

#include "Scripts/EntityCommand/ICommand.h"
#include "Scripts/IEntity/IEntity.h"

class IEntityCommandHandler {
public:
	virtual ~IEntityCommandHandler() = default;

public:
	virtual void initialize(Reference<IEntity> receiver_);
	virtual void begin() = 0;
	virtual void update() = 0;

public:
	void run();

protected:
	void entry_command(u64 commandIndex);

protected:
	std::vector<std::unique_ptr<ICommand>> commands;
	std::list<std::function<void(void)>> commandEntries;
	Reference<IEntity> receiver;
};
