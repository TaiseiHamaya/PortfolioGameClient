#include "Portfolio.h"

#include "Scripts/Proto/types.pb.h"

#include <Engine/Runtime/Scene/SceneManager.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Application/WinApp.h>
#include <Engine/Debug/Editor/EditorMain.h>

#ifdef ENABLE_DEVELOP_BOT
#include <Engine/Application/ProjectSettings/ProjectSettings.h>
#include <Engine/Application/Output.h>
#endif // ENABLE_DEVELOP_BOT

void Portfolio::initialize() {
#ifdef DEBUG_FEATURES_ENABLE
	EditorMain::SetActiveEditor(false);
#endif // DEBUG_FEATURES_ENABLE

	SceneManager::Setup(std::make_unique<FactoryPortfolio>());

#ifdef ENABLE_DEVELOP_BOT
	ProjectSettings::SetMaxFrameRate(60);
	Information(L"Botモードでビルドされています");
#endif // ENABLE_DEVELOP_BOT
}

void Portfolio::finalize() {
	google::protobuf::ShutdownProtobufLibrary();

	WinApp::Finalize();
}
