#include "DisconnectedScene.h"

#include "Scripts/ScriptDisconnected/DisconnectedScript.h"

DisconnectedScene::DisconnectedScene() noexcept {
	sceneName = "DisconnectedScene";
}

void DisconnectedScene::custom_setup() {
	sceneScriptManager.register_script(
		std::make_unique<DisconnectedScript>()
	);
}
