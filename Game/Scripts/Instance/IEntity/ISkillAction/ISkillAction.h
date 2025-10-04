#pragma once

#include <Engine/Runtime/Clock/WorldTimer.h>

#include "../IActionBasic.h"
#include "Scripts/Instance/IEntity/IEntity.h"

#include "Scripts/Manager/EffectManager.h"

class ISkillAction : public IActionBasic {
public:
	enum class TargetType : i8 {
		Target,
		Ground,
		Self,
	};

	enum class SkillType : i8 {
		WeaponSkill,
		Ability,
		Spell,
	};

public:
	ISkillAction() noexcept = default;
	virtual ~ISkillAction() = default;

public:


protected:
	TargetType targetType{};
	SkillType skillType{};
	WorldTimer castTime;
	WorldTimer recastTime;
	i32 mpCost{};
	float range{};
	float radius{};

public:
	static void SetEffectManager(Reference<EffectManager> manager);

protected:
	static inline Reference<EffectManager> effectManager{ nullptr };
};
