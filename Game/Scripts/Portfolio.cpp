#include "Portfolio.h"

#include "google/protobuf/message_lite.h"

#include <Engine/Runtime/Scene/SceneManager2.h>

#include "./Scene/FactoryPortfolio.h"
#include "Network/NetworkCluster.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Application/ArgumentParser.h>
#include <Engine/Application/ProjectSettings/ProjectSettings.h>
#include <Engine/Application/Logger.h>
#endif // DEBUG_FEATURES_ENABLE

void Portfolio::initialize() {
	szg::SceneManager2::Setup(std::make_unique<FactoryPortfolio>());

#ifdef DEBUG_FEATURES_ENABLE
	if (szg::ArgumentParser::Contains("--develop-bot-enable")) {
		szg::ProjectSettings::SetMaxFrameRate(60);
		szg::ProjectSettings::GetApplicationSettingsMut().hideWindowForce = true;
		szgInformation(L"Botモードが有効化されました。");
	}
#endif // DEBUG_FEATURES_ENABLE

	ProtoBufManager::GetInstance().onDestroy;
}

Portfolio::ProtoBufManager::OnDestroy::~OnDestroy() {
	NetworkCluster::Finalize();
	google::protobuf::ShutdownProtobufLibrary();
}
