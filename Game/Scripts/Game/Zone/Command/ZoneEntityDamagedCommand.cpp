#include "ZoneEntityDamagedCommand.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/MiscInstance/Effects/DamageNumberEffect.h"

ZoneEntityDamagedCommand::ZoneEntityDamagedCommand(Reference<IEntity> entity_, const u32 damage_) {
	entity = entity_;
	damage = damage_;
	effectPosition = EffectTools::CalculateTargetPosition(cameraInstance, entity);
}

void ZoneEntityDamagedCommand::execute() {
	if (!entity) {
		return;
	}
	entity->on_damaged(damage);

	if (effectManager) {
		Reference<szg::WorldRoot> world = entity->world_root_mut();
		
		Reference<DamageNumberEffect> damageEffect = world->instantiate<DamageNumberEffect>(nullptr);
		damageEffect->initialize(static_cast<i32>(damage), DamageColor);
		damageEffect->transform_mut().set_translate(effectPosition);

		effectManager->register_instance(damageEffect);
	}
}
