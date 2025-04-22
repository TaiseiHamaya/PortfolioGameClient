#include "FactoryPortfolio.h"

#include <Engine/Runtime/Scene/BaseScene.h>

#include "Portfolio/Scene/SceneGame.h"

std::unique_ptr<BaseScene> FactoryPortfolio::initialize_scene() {
	return std::make_unique<SceneGame>();
}

std::unique_ptr<BaseScene> FactoryPortfolio::create_scene([[maybe_unused]] int32_t next) {
	return std::make_unique<SceneGame>();
}
