#include "Portfolio.h"

#include <Engine/Runtime/Scene/SceneManager.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Debug/Editor/EditorMain.h>

void Portfolio::initialize() {
	EditorMain::SetActiveEditor(false);

	SceneManager::Setup(std::make_unique<FactoryPortfolio>());
}
