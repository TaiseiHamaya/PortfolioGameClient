#include "FactoryPortfolio.h"

#include "Scripts/Scene/SceneGame.h"
#include "Scripts/Scene/LoginScene.h"

std::unique_ptr<szg::Scene> FactoryPortfolio::initialize_scene2() {
	return create_scene2(SceneListPortfolio::SCENE_LOGIN);
}

std::unique_ptr<szg::Scene> FactoryPortfolio::create_scene2(i32 next) {
	switch (next) {
	case SCENE_GAME:
		return std::make_unique<SceneGame>();
	case SCENE_LOGIN:
		return std::make_unique<LoginScene>();
	default:
		return nullptr;
	}
}
