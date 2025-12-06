#include "FactoryPortfolio.h"

#include <Engine/Runtime/Scene/BaseScene.h>

#include "Scripts/Scene/SceneGame.h"

std::unique_ptr<szg::BaseScene> FactoryPortfolio::initialize_scene() {
	return nullptr;
}

std::unique_ptr<szg::BaseScene> FactoryPortfolio::create_scene([[maybe_unused]] int32_t next) {
	return nullptr;
}

std::unique_ptr<szg::Scene> FactoryPortfolio::initialize_scene2() {
	return create_scene2(0);
}

std::unique_ptr<szg::Scene> FactoryPortfolio::create_scene2(int32_t) {
	return std::make_unique<SceneGame>();
}
