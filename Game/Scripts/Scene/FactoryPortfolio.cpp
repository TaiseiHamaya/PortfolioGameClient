#include "FactoryPortfolio.h"

#include "Scripts/Scene/DisconnectedScene.h"
#include "Scripts/Scene/LoginScene.h"
#include "Scripts/Scene/SceneGame.h"

#include <Engine/Application/ArgumentParser.h>

std::unique_ptr<szg::Scene> FactoryPortfolio::initialize_scene2() {
	if (szg::ArgumentParser::Contains("--develop-bot-enable")) {
		return create_scene2(SceneListPortfolio::SCENE_GAME);
	}
	return create_scene2(SceneListPortfolio::SCENE_LOGIN);
}

std::unique_ptr<szg::Scene> FactoryPortfolio::create_scene2(i32 next) {
	switch (next) {
	case SCENE_LOGIN:
		return std::make_unique<LoginScene>();
	case SCENE_GAME:
		return std::make_unique<SceneGame>();
	case SCENE_DISCONNECTED:
		return std::make_unique<DisconnectedScene>();
	default:
		return nullptr;
	}
}
