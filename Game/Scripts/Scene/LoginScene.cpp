#include "LoginScene.h"

#include <Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h>

LoginScene::LoginScene() noexcept {
	sceneName = "LoginScene";
}

void LoginScene::custom_load_asset() {
	szg::FontAtlasMSDFLibrary::RegisterLoadQue("./Game/Resources/Font/UDEVGothic35HS-Regular.mtsdf");
}

void LoginScene::custom_setup() {
}
