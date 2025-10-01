#include "GameServerConnectionManager.h"

#include <Scripts/Proto/types.pb.h>

#include <Engine/Application/Output.h>

constexpr string_literal LOCAL_LOOPBACK_ADDRESS = "127.0.0.1";

void GameServerConnectionManager::initialize() {
	attach_context_thread();
}

void GameServerConnectionManager::connect() {
	asio::ip::tcp::endpoint endpoint{
		asio::ip::make_address(LOCAL_LOOPBACK_ADDRESS),
		3215
	};
	// 非同期接続要求
	socket.async_connect(endpoint, [&](const asio::error_code& error) {
		on_connect_handler(error);
	});
	connectionState = ConnectionState::ConnectionRequested;
}

void GameServerConnectionManager::disconnect() {
	if (is_established()) {
		socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
	}
	socket.close();
	connectionState = ConnectionState::DisconnectRequest;
}

void GameServerConnectionManager::finalize() {
}

void GameServerConnectionManager::update() {
	std::unique_lock lock{ mutex };
	if (isThreadEnded) {
		attach_context_thread();
	}
}

void GameServerConnectionManager::on_connection_succussed() {
	connectionState = ConnectionState::ConnectionComplete;
}

bool GameServerConnectionManager::is_established() const {
	return
		connectionState == ConnectionState::ConnectionComplete ||
		connectionState == ConnectionState::ConnectionEstablished;
}

bool GameServerConnectionManager::is_connected() const {
	return connectionState == ConnectionState::ConnectionComplete;
}

asio::ip::tcp::socket& GameServerConnectionManager::get_socket() {
	return socket;
}

const asio::ip::tcp::socket& GameServerConnectionManager::get_socket() const {
	return socket;
}

void GameServerConnectionManager::on_connect_handler(const asio::error_code& errorCode) {
	if (!errorCode) {
		// Successfully connected to the server
		Information("Connected to the server successfully.");
		if (connectionState != ConnectionState::ConnectionComplete) {
			connectionState = ConnectionState::ConnectionEstablished;
		}
	}
	else {
		// Handle connection error
		Error("Failed to connect to the server. Message-\'{}\'", errorCode.message());
	}
}

void GameServerConnectionManager::attach_context_thread() {
	if (contextThread.joinable()) {
		contextThread.join();
	}
	contextThread = std::thread{ [&]() {
		context.run();
		std::lock_guard lock{ mutex };
		isThreadEnded = true;
	} };
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#include <imgui_stdlib.h>
void GameServerConnectionManager::debug_gui() {
	ImGui::Begin("GameServerConnectionManager");
	ImGui::Text("Connected: %s", is_connected() ? "Yes" : "No");
	ImGui::Text("Connection State: %d", static_cast<i32>(connectionState));
	if (ImGui::Button("Connect")) {
		connect();
	}
	if (ImGui::Button("Disconnect")) {
		disconnect();
	}
}
#endif // DEBUG_FEATURES_ENABLE