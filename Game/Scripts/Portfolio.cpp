#include "Portfolio.h"

#include "Scripts/Proto/types.pb.h"

#include <Engine/Runtime/Scene/SceneManager.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Application/WinApp.h>
#include <Engine/Debug/Editor/EditorMain.h>

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Application/ArgumentParser.h>
#include <Engine/Application/ProjectSettings/ProjectSettings.h>
#include <Engine/Application/Output.h>
#endif // DEBUG_FEATURES_ENABLE

void Portfolio::initialize() {
#ifdef DEBUG_FEATURES_ENABLE
	EditorMain::SetActiveEditor(false);
#endif // DEBUG_FEATURES_ENABLE

	SceneManager::Setup(std::make_unique<FactoryPortfolio>());

#ifdef DEBUG_FEATURES_ENABLE
	if (ArgumentParser::Contains("-ENABLE_DEVELOP_BOT")) {
		ProjectSettings::SetMaxFrameRate(60);
		ProjectSettings::GetApplicationSettingsImm().hideWindowForce = true;
		Information(L"Botモードが有効化されました。");
	}
#endif // DEBUG_FEATURES_ENABLE
}

void Portfolio::finalize() {
	google::protobuf::ShutdownProtobufLibrary();

	WinApp::Finalize();
}
