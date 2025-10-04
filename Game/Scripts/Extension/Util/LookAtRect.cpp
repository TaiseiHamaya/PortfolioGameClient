#include "LookAtRect.h"

void LookAtRect::fixed_update() {
	look_at(*camera.ptr());
}
