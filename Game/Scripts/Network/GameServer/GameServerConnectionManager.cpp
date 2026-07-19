#include "GameServerConnectionManager.h"

#include <Engine/Application/Logger.h>

void GameServerConnectionManager::initialize() {
	isThreadEnded = false;
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

void GameServerConnectionManager::setup(const std::string& address, u16 port) {
	endpoint = {
		asio::ip::make_address(address),
		port
	};
}

void GameServerConnectionManager::connect() {
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
		std::lock_guard lock{ mutex };
		socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
		connectionState = ConnectionState::DisconnectRequest;
		socket.close();
	}
	std::lock_guard lock{ mutex };
	connectionState = ConnectionState::Disconnected;
}

void GameServerConnectionManager::finalize() {
	timer.cancel();
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

bool GameServerConnectionManager::is_disconnected() const {
	std::lock_guard lock{ mutex };
	return connectionState == ConnectionState::Disconnected || connectionState == ConnectionState::DisconnectRequest;
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
			szgWarning("サーバーとの接続がタイムアウトしました。");
		}
		else {
			switch (errorCode.value()) {
			case asio::error::connection_refused:
				szgWarning("サーバーと接続しましたが、拒否されました。");
				break;
			case asio::error::operation_aborted:
				szgWarning("サーバーとの接続は中止されました。");
				break;
			case asio::error::already_connected:
				szgWarning("サーバーとの既に接続されています。");
				break;
			default:
				szgWarning("不明な理由でサーバーとの接続に失敗しました。: ErrorCode-\'{}\'", errorCode.message());
				break;
			}
		}
		connectionState = ConnectionState::Disconnected;
		// ログウィンドウに表示
		// TODO: mutexがいるのでどうにかする
	}
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#include <imgui_stdlib.h>
void GameServerConnectionManager::debug_gui() {
	ImGui::Text("Connected: %s", is_established() ? "Yes" : "No");
	ImGui::Text("Connection State: %d", static_cast<i32>(connectionState));
	if (ImGui::Button("Connect")) {
		connect();
	}
	if (ImGui::Button("Disconnect")) {
		disconnect();
	}
}
#endif // DEBUG_FEATURES_ENABLE