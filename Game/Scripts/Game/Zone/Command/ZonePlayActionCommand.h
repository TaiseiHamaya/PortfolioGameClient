#pragma once

#include "./IZoneCommand.h"

#include <Library/Utility/Template/Reference.h>

class IEntity;

/// <summary>
/// アクション実行コマンド
/// </summary>
class ZonePlayActionCommand final : public IZoneCommand {
public:
	ZonePlayActionCommand(Reference<IEntity> entity_, const u32 actionId_, Reference<IEntity> targetEntity_);

public:
	void execute(Zone& zone) override;

private:
	Reference<IEntity> entity;
	u32 actionId;
	Reference<IEntity> targetEntity;
};
