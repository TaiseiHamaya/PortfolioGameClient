#include "GameServerConnectionManager.h"

#include <Engine/Application/Output.h>

constexpr string_literal LOCAL_LOOPBACK_ADDRESS = "127.0.0.1";

#include "Scripts/Proto/types.pb.h"

void GameServerConnectionManager::initialize() {
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

	context.poll();
}

void GameServerConnectionManager::disconnect() {
	if (is_connected()) {
		socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
	}
	socket.close();
	isConnected = false;
}

void GameServerConnectionManager::finalize() {
}

bool GameServerConnectionManager::is_connected() const {
	return isConnected;
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
		isConnected = true;
	}
	else {
		// Handle connection error
		Error("Failed to connect to the server. Message-\'{}\'", errorCode.message());
	}
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>

void GameServerConnectionManager::debug_gui() {
	ImGui::Begin("GameServerConnectionManager");
	ImGui::Text("Connected: %s", is_connected() ? "Yes" : "No");
	if (ImGui::Button("Connect")) {
		connect();
	}
	if (ImGui::Button("Disconnect")) {
		disconnect();
	}

	if (ImGui::Button("Test Send Packet")) {
		Proto::Packet packet;
		packet.set_loginpackettype(Proto::LoginRequest);

		socket.async_send(
			asio::buffer(packet.SerializeAsString()),
			[](const asio::error_code& error, std::size_t bytes_transferred) {
			if (!error) {
				Information("Sent {} bytes to the server.", bytes_transferred);
			}
			else {
				Error("Failed to send packet to the server. Message-\'{}\'", error.message());
			}
		});
	}
	ImGui::End();
}
#endif // DEBUG_FEATURES_ENABLE