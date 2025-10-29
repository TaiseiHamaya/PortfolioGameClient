#include "PaladinHolySpirit.h"

#include <Engine/Module/World/WorldManager.h>

#include "PaladinHolySpiritEffectSelf.h"
#include "PaladinHolySpiritEffectTarget.h"

PaladinHolySpirit::PaladinHolySpirit() noexcept {
	targetType = TargetType::Target;
	skillType = SkillType::Spell;
	castTime.set(1.5f);
	recastTime.set(2.5f);
	mpCost = 1000;
	range = 25.0f;
	radius = 0.0f;
	timer.set(0);
	effect = ActionEffect::Spell;
	loopAnimation = false;
	type = ActionType::Spell;
}

void PaladinHolySpirit::start() {
	auto temp = owner->world_manager()->create<PaladinHolySpiritEffectTarget>();
	temp->initialize(EffectTools::CalculateTargetPosition(owner, owner->get_selection_target()));
	effectManager->register_instance(std::move(temp));
}

void PaladinHolySpirit::update() {
	timer.ahead();
}

void PaladinHolySpirit::reset() {
	timer.set(0);
}

bool PaladinHolySpirit::can_transition() const {
	return timer >= 1.8f;
}

bool PaladinHolySpirit::end_action() const {
	return timer >= 2.5f;
}
