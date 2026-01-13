#pragma once

#include "Engine/Runtime/Scene/BaseSceneFactory.h"

enum SceneListPortfolio {
	SCENE_LOGIN,
	SCENE_GAME,
};

class FactoryPortfolio final : public szg::BaseSceneFactory {
public:
	std::unique_ptr<szg::Scene> initialize_scene2() override;
	std::unique_ptr<szg::Scene> create_scene2(i32 next) override;
};
