#include "IEffectInstance.h"

Vector3 EffectTools::CalculateTargetPosition(Reference<const szg::WorldInstance> self, Reference<const IEntity> target) {
	Vector3 worldPosition{};
	Vector3 offset = Vector3{ 0.0f, 1.5f, 1.0f };
	if (target) {
		worldPosition = target->world_position();
		offset *= target->target_radius();
	}
	Vector3 result{};
	if (self && target) {
		// 自分の方向に移動
		Vector3 selfBackward = self->world_position() - target->world_position();
		selfBackward.y = 0.0f;
		selfBackward = selfBackward.normalize_safe();
		result = worldPosition + offset * Quaternion::LookForward(selfBackward);
	}
	return result;
}

bool IEffectInstance::is_end_effect() const {
	return isEnded;
}
