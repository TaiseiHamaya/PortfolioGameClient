#pragma once

#include "./IZoneCommand.h"

#include <string>

#include <Library/Utility/Template/Reference.h>

class EntityManager;

class ZoneLoginPlayerCommand final : public IZoneCommand {
public:
	ZoneLoginPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId, const std::string& name_);

public:
	void execute() override;

private:
	Reference<EntityManager> entityManager;
	u64 serverId;
	std::string name;
};
