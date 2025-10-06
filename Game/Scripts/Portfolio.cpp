#include "Portfolio.h"

#include <Engine/Runtime/Scene/SceneManager.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Application/WinApp.h>
#include <Engine/Debug/Editor/EditorMain.h>

#include <google/protobuf/message_lite.h>

void Portfolio::initialize() {
#ifdef DEBUG_FEATURES_ENABLE
	EditorMain::SetActiveEditor(false);
#endif // DEBUG_FEATURES_ENABLE

	SceneManager::Setup(std::make_unique<FactoryPortfolio>());
}

void Portfolio::finalize() {
	google::protobuf::ShutdownProtobufLibrary();

	WinApp::Finalize();
}
