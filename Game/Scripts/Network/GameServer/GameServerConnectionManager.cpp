#include "GameServerConnectionManager.h"

#include <Engine/Application/Logger.h>

static constexpr string_literal LOCAL_LOOPBACK_ADDRESS = "127.0.0.1";
static constexpr string_literal AWS_SERVER_ADDRESS = "18.180.254.93";

void GameServerConnectionManager::initialize() {
	contextThread = std::thread{ [&]() {
		while (true) {
			{
				std::unique_lock lock{ mutex };
				if (isThreadEnded) {
					break;
				}
			}
			std::this_thread::sleep_for(
				std::chrono::milliseconds(100)
			);
			context.run();
		}
	} };
	SetThreadDescription(contextThread.native_handle(), L"Asio Context Thread");
}

void GameServerConnectionManager::connect() {
	endpoint = {
		asio::ip::make_address(AWS_SERVER_ADDRESS),
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
	{
		std::lock_guard<std::mutex> lock{ mutex };
		isThreadEnded = true;
	}
	if (contextThread.joinable()) {
		contextThread.join();
	}
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
			szgError("サーバーとの接続がタイムアウトしました。");
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