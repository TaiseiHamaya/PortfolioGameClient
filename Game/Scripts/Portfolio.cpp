#include "Portfolio.h"

#include "Scripts/Proto/types.pb.h"

#include <Engine/Runtime/Scene/SceneManager.h>
#include <Engine/Runtime/Scene/SceneManager2.h>

#include "./Scene/FactoryPortfolio.h"

#include <Engine/Application/WinApp.h>
#include <Engine/Debug/Editor/EditorMain.h>

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Application/ArgumentParser.h>
#include <Engine/Application/ProjectSettings/ProjectSettings.h>
#include <Engine/Application/Logger.h>
#endif // DEBUG_FEATURES_ENABLE

void Portfolio::initialize() {
#ifdef DEBUG_FEATURES_ENABLE
	//EditorMain::SetActiveEditor(false);
#endif // DEBUG_FEATURES_ENABLE

	SceneManager2::Setup(std::make_unique<FactoryPortfolio>());

#ifdef DEBUG_FEATURES_ENABLE
	if (ArgumentParser::Contains("--develop-bot-enable")) {
		ProjectSettings::SetMaxFrameRate(60);
		ProjectSettings::GetApplicationSettingsMut().hideWindowForce = true;
		szgInformation(L"Botモードが有効化されました。");
	}
#endif // DEBUG_FEATURES_ENABLE

	ProtoBufManager::GetInstance().onDestroy;
}

Portfolio::ProtoBufManager::OnDestory::~OnDestory() {
	google::protobuf::ShutdownProtobufLibrary();
}
