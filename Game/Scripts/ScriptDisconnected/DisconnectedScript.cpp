#include "DisconnectedScript.h"

#include <Engine/Runtime/Scene/SceneManager2.h>

void DisconnectedScript::prev_update() {
	timer.back();

	if (timer <= 1.0f) {
#ifndef DEBUG_FEATURES_ENABLE
		szg::SceneManager2::SceneChange(4, 0.0f);
#endif
	}
}
