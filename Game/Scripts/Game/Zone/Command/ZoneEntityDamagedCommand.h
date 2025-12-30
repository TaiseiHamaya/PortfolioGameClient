#pragma once

#include "./IZoneCommand.h"

#include <Library/Math/Color4.h>
#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>

namespace szg {

class WorldInstance;

};

class IEntity;

/// <summary>
/// ダメージ処理コマンド
/// </summary>
class ZoneEntityDamagedCommand final : public IZoneCommand {
public:
	ZoneEntityDamagedCommand(Reference<IEntity> entity_, const u32 damage_);

public:
	void execute() override;

private:
	Reference<IEntity> entity;

	u32 damage;

	Vector3 effectPosition;

public:
	static inline Reference<class EffectManager> effectManager{};
	static inline Reference<const szg::WorldInstance> cameraInstance{};
	static inline Color4 DamageColor{ 0xE02929FF };
};
