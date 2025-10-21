#include "GameServerConnectionManager.h"

#include <Engine/Application/Logger.h>

constexpr string_literal LOCAL_LOOPBACK_ADDRESS = "127.0.0.1";

void GameServerConnectionManager::initialize() {
	attach_context_thread();
}

void GameServerConnectionManager::connect() {
	endpoint = {
		asio::ip::make_address(LOCAL_LOOPBACK_ADDRESS),
		3215
	};
	// 非同期接続要求
	// タイムアウト処理
	timer.expires_after(std::chrono::seconds{ 5 });
	timer.async_wait([&](const asio::error_code& error) {
		if (!error) {
			socket.cancel();
			std::lock_guard lock{ mutex };
			connectionState = ConnectionState::Disconnected;
		}
	});
	// 接続要求
	socket.async_connect(endpoint, [&](const asio::error_code& errorCode) {
		if (!errorCode) {
			timer.cancel();
		}
		on_connect_handler(errorCode);
	});
	// 接続要求送信済み
	std::lock_guard lock{ mutex };
	connectionState = ConnectionState::ConnectionRequested;
	attach_context_thread();
}

void GameServerConnectionManager::disconnect() {
	if (is_established()) {
		socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
	}
	socket.close();
	std::lock_guard lock{ mutex };
	connectionState = ConnectionState::DisconnectRequest;
}

void GameServerConnectionManager::finalize() {
	context.stop();
	if (contextThread.joinable()) {
		contextThread.join();
	}
}

void GameServerConnectionManager::update() {
	std::lock_guard lock{ mutex };
	attach_context_thread();
}

void GameServerConnectionManager::on_connection_succeeded() {
	std::lock_guard lock{ mutex };
	connectionState = ConnectionState::ConnectionComplete;
}

bool GameServerConnectionManager::is_established() const {
	std::lock_guard lock{ mutex };
	return
		connectionState == ConnectionState::ConnectionComplete ||
		connectionState == ConnectionState::ConnectionEstablished;
}

bool GameServerConnectionManager::is_connected() const {
	std::lock_guard lock{ mutex };
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
		szgInformation("Connected to the server successfully.");
		std::lock_guard lock{ mutex };
		if (connectionState != ConnectionState::ConnectionComplete) {
			connectionState = ConnectionState::ConnectionEstablished;
		}
	}
	else {
		// Handle connection error
		std::lock_guard lock{ mutex };
		if (connectionState == ConnectionState::Disconnected) {
			szgError("サーバーとの接続はタイムアウトしました。");
		}
		else {
			switch (errorCode.value()) {
			case asio::error::connection_refused:
				szgError("サーバーと接続しましたが、拒否されました。");
				break;
			case asio::error::operation_aborted:
				szgError("サーバーとの接続は中止されました。");
				break;
			case asio::error::already_connected:
				szgError("サーバーとの既に接続されています。");
				break;
			default:
				szgError("不明な理由でサーバーとの接続に失敗しました。: ErrorCode-\'{}\'", errorCode.message());
				break;
			}
		}
		connectionState = ConnectionState::Disconnected;
	}
}

void GameServerConnectionManager::attach_context_thread() {
	if (!isThreadEnded) {
		return;
	}
	if (contextThread.joinable()) {
		contextThread.join();
	}
	isThreadEnded = false;
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