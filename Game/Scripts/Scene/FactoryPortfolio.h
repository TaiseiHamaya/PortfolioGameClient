#pragma once

#include "Engine/Runtime/Scene/BaseSceneFactory.h"

enum SceneListPortfolio {
	SCENE_GAME,
};

class FactoryPortfolio final : public BaseSceneFactory {
public:
	std::unique_ptr<BaseScene> initialize_scene();
	std::unique_ptr<BaseScene> create_scene(int32_t next);

public:
	std::unique_ptr<Scene> initialize_scene2() override;
	std::unique_ptr<Scene> create_scene2(int32_t next) override;
};

