#pragma once

#include "./IZoneCommand.h"

#include <Library/Math/ColorRGBA.h>
#include <Library/Utility/Template/Reference.h>

class IEntity;

/// <summary>
/// ダメージ処理コマンド
/// </summary>
class ZoneEntityDamagedCommand final : public IZoneCommand {
public:
	ZoneEntityDamagedCommand(Reference<IEntity> entity_, const u32 damage_);

public:
	void execute(Zone& zone) override;

private:
	Reference<IEntity> entity;

	u32 damage;

public:
	static inline ColorRGBA DamageColor{ 0xE02929FF };
};
