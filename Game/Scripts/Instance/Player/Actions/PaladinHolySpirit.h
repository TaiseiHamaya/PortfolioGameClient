#pragma once

#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"

class PaladinHolySpirit final : public ISkillAction {
public:
	PaladinHolySpirit() noexcept;
	~PaladinHolySpirit() = default;

public:
	void start() override;
	void update() override;
	void reset() override;

public:
	bool can_transition() const override;
	bool end_action() const override;

	//std::vector<std::unique_ptr<IEffectInstance>> on_impact(Reference<IEntity> entity, Reference<IEntity> target, Reference<WorldManager> world) override;
};
