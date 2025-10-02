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

//std::vector<std::unique_ptr<IEffectInstance>> PaladinHolySpirit::on_impact(Reference<IEntity> entity, Reference<IEntity> target, Reference<WorldManager> world) {
//	std::vector<std::unique_ptr<IEffectInstance>> result{};
//
//	// ホリスピ強化状態化のチェックと消費処理
//	//bool isEnchantedHolySpirit = entity;
//
//	// 敵へのダメージ
//	if (target) {
//		target->on_damaged(100);
//	}
//
//	// エフェクトの生成
//	auto temp = world->create<PaladinHolySpiritEffectTarget>();
//	temp->initialize(EffectTools::CalculateTargetPosition(entity, target));
//	result.emplace_back(std::move(temp));
//
//	return result;
//}
