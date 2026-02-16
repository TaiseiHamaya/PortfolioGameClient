#include "LoginScene.h"

#include <Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h>

#include "Scripts/ScriptTitle/TitleStateScript.h"

LoginScene::LoginScene() noexcept {
	sceneName = "LoginScene";
}

void LoginScene::custom_load_asset() {
	szg::FontAtlasMSDFLibrary::RegisterLoadQue("UDEVGothic35HS-Regular.mtsdf");
}

void LoginScene::custom_setup() {
	auto titleStateScript = std::make_unique<TitleStateScript>();

	titleStateScript->setup();

	sceneScriptManager.register_script(std::move(titleStateScript));
}
