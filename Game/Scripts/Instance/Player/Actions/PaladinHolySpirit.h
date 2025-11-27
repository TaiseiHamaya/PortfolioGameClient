#pragma once

#include <memory>

#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"

class PaladinHolySpiritEffectTarget;

class PaladinHolySpirit final : public ISkillAction {
public:
	PaladinHolySpirit() noexcept;
	~PaladinHolySpirit() noexcept;

public:
	void setup(Reference<IEntity> owner_, const std::string& animationName) override;

public:
	void start() override;
	void update() override;
	void reset() override;

public:
	bool can_transition() const override;
	bool end_action() const override;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;

private:
	std::unique_ptr<PaladinHolySpiritEffectTarget> effectTarget;
#endif // DEBUG_FEATURES_ENABLE
};
