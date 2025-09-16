#include "IEffectInstance.h"

Vector3 EffectTools::CalculateTargetPosition(Reference<IEntity> self, Reference<IEntity> target) {
	Vector3 worldPosition{};
	Vector3 offset = Vector3{ 0.0f, 1.5f, 1.0f };
	if (target) {
		worldPosition = target->world_position();
		offset *= target->target_radius();
	}
	Vector3 result{};
	if (self && target) {
		Vector3 selfBackward = self->world_position() - target->world_position();
		selfBackward.y = 0.0f;
		selfBackward = selfBackward.normalize_safe();
		result = worldPosition + offset * Quaternion::LookForward(selfBackward);
	}
	return result;
}

bool IEffectInstance::is_destroy() const {
	return isDestroy;
}
