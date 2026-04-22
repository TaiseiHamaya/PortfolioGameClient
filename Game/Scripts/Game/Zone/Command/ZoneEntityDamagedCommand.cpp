#include "ZoneEntityDamagedCommand.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

#include "Scripts/Game/Zone/Zone.h"
#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/MiscInstance/Effects/DamageNumberEffect.h"
#include "Scripts/Instance/MiscInstance/Effects/IEffectInstance.h"

ZoneEntityDamagedCommand::ZoneEntityDamagedCommand(Reference<IEntity> entity_, const u32 damage_) :
	entity(entity_),
	damage(damage_) {
}

void ZoneEntityDamagedCommand::execute(Zone& zone) {
	if (!entity) {
		return;
	}
	entity->on_damaged(damage);

	Reference<EffectManager> effectManager = zone.effect_manager_mut();
	if (effectManager) {
		Vector3 effectPosition = EffectTools::CalculateTargetPosition(zone.camera_instance_imm(), entity);
		Reference<szg::WorldRoot> world = entity->world_root_mut();

		Reference<DamageNumberEffect> damageEffect = world->instantiate<DamageNumberEffect>(nullptr);
		damageEffect->initialize(static_cast<i32>(damage), DamageColor);
		damageEffect->transform_mut().set_translate(effectPosition);

		effectManager->register_instance(damageEffect);
	}
}
