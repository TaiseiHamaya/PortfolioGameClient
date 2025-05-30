#pragma once

#include "Scripts/IEntity/ISkillAction/ISkillAction.h"

class PaladinHolySpirit final : public ISkillAction {
public:
	PaladinHolySpirit() noexcept;
	~PaladinHolySpirit() = default;

public:
	std::vector<std::unique_ptr<IEffectInstance>> on_impact(Reference<IEntity> entity, Reference<IEntity> target, Reference<WorldManager> world) override;
};
