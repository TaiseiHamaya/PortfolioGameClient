#include "LookAtRect.h"

void LookAtRect::fixed_update() {
	Vector3 cameraBackward = CVector3::BACKWARD * camera->world_affine().get_basis().to_quaternion();

	Vector3 lookAt = world_position() + cameraBackward;
	look_at(lookAt);
}
