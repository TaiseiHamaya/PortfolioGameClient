#pragma once

#include "../ICommand.h"

class MoveCommand final : public ICommand {
public:
	MoveCommand(const Vector2& dir) : xzDirection(dir) {};
	~MoveCommand() = default;

	void execute(Reference<IEntity> entity);

private:
	const Vector2& xzDirection;
};
