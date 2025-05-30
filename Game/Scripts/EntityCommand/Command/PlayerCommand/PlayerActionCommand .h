#pragma once

#include "Scripts/EntityCommand/ICommand.h"

#include <memory>

#include "Scripts/IEntity/ISkillAction/ISkillAction.h"

class PlayerActionCommand final : public ICommand {
public:
	PlayerActionCommand(std::unique_ptr<ISkillAction> skillAction_);
	~PlayerActionCommand() override = default;

public:
	void execute(Reference<IEntity> entity) override;

private:
	std::unique_ptr<ISkillAction> skillAction;
};

