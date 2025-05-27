#pragma once

#include <Engine/Runtime/Clock/WorldTimer.h>

#include "Scripts/IEntity/IEntity.h"

class ISkillAction {
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
	virtual std::vector<std::unique_ptr<WorldInstance>> on_impact(Reference<IEntity> entity, Reference<IEntity> target, Reference<WorldManager> world) = 0;

protected:
	TargetType targetType{};
	SkillType skillType{};
	WorldTimer castTime;
	WorldTimer recastTime;
	i32 mpCost{};
	float range{};
	float radius{};
};
