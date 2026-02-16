#include "LoginScene.h"

#include "Scripts/ScriptTitle/TitleStateScript.h"

LoginScene::LoginScene() noexcept {
	sceneName = "LoginScene";
}

void LoginScene::custom_setup() {
	auto titleStateScript = std::make_unique<TitleStateScript>();

	titleStateScript->setup();

	sceneScriptManager.register_script(std::move(titleStateScript));
}
