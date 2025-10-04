#pragma once

#include "./IZoneCommand.h"

#include <Library/Utility/Template/Reference.h>

class EntityManager;

class ZoneLogoutPlayerCommand final : public IZoneCommand {
public:
	ZoneLogoutPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId_);

public:
	void execute() override;

private:
	Reference<EntityManager> entityManager;
	u64 serverId;
};
