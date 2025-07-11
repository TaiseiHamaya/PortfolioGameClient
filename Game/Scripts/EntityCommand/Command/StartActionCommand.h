#pragma once

#include "../ICommand.h"

class StartActionCommand final : public ICommand {
public:
	StartActionCommand(std::string_view name);
	~StartActionCommand() = default;

	void execute(Reference<IEntity> entity);

private:
	eps::string_hashed actionName;
};
