#pragma once

#include "./IZoneCommand.h"

#include <string>

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>

class EntityManager;

/// <summary>
/// ログイン処理
/// </summary>
class ZoneLoginPlayerCommand final : public IZoneCommand {
public:
	ZoneLoginPlayerCommand(Reference<EntityManager> entityManager_, u64 serverId, const std::string& name_, const Vector3& position_);

public:
	void execute() override;

private:
	Reference<EntityManager> entityManager;
	u64 serverId;
	std::string name;
	Vector3 position;
};
