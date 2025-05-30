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
}

std::vector<std::unique_ptr<WorldInstance>> PaladinHolySpirit::on_impact(Reference<IEntity> entity, Reference<IEntity> target, Reference<WorldManager> world) {
	std::vector<std::unique_ptr<WorldInstance>> result{};

	// ホリスピ強化状態化のチェックと消費処理
	//bool isEnchantedHolySpirit = entity;

	// 敵へのダメージ
	if (target) {
		target->on_damaged(100);
	}

	// エフェクトの生成
	auto temp = world->create<PaladinHolySpiritEffectTarget>();
	temp->initialize();
	temp->setup(entity, target);
	result.emplace_back(std::move(temp));

	return result;
}
