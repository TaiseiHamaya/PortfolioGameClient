#include "LoginScene.h"

#include <Engine/Application/ArgumentParser.h>

#include "Scripts/Network/NetworkCluster.h"
#include "Scripts/ScriptTitle/TitleStateScript.h"
#include "Scripts/Network/ProtoBufManager.h"

static constexpr string_literal LOCAL_LOOPBACK_ADDRESS = "127.0.0.1";
static constexpr string_literal AWS_SERVER_ADDRESS = "18.180.254.93";
static constexpr u16 SERVER_PORT = 3215;

LoginScene::LoginScene() noexcept {
	sceneName = "LoginScene";
}

void LoginScene::custom_setup() {
	static bool isNetworkClusterInitialized = false;
	if (!isNetworkClusterInitialized) {
		isNetworkClusterInitialized = true;
		NetworkCluster::Initialize();
	}
	else {
		NetworkCluster::Finalize();
		NetworkCluster::Initialize();
	}

	std::string prefix = "-server-addr=";
	std::optional<std::string> serverAddressOpt = szg::ArgumentParser::FindValueStartWith(prefix, 1);
	if (serverAddressOpt.has_value()) {
		NetworkCluster::Setup(serverAddressOpt.value().substr(prefix.size()), SERVER_PORT);
	}
	else {
		NetworkCluster::Setup(AWS_SERVER_ADDRESS, SERVER_PORT);
	}

	NetworkCluster::Connect();

	auto titleStateScript = std::make_unique<TitleStateScript>();

	titleStateScript->setup();

	sceneScriptManager.register_script(std::move(titleStateScript));

	ProtoBufManager::Allocate();
}
