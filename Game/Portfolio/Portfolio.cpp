#include "Portfolio.h"

#include "Engine/Runtime/Scene/SceneManager.h"

#include "./Scene/FactoryPortfolio.h"

void Portfolio::initialize() {
	SceneManager::SetFactory<FactoryPortfolio>();

	Framework::initialize();
}

