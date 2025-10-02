#pragma once

#include <thread>

#include <asio.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

class GameServerConnectionManager final {
public:
	enum class ConnectionState {
		Disconnected, // 切断されている状態

		ConnectionRequested, // 接続要求を送信した状態
		ConnectionEstablished, // TCPレベルで接続が確立した状態
		ConnectionComplete, // アプリケーションレベルで接続が確立した状態

		DisconnectRequest, // 切断要求を送信した状態
		DisconnectEstablished, // アプリケーションレベルで切断が確立した状態
	};

public:
	GameServerConnectionManager() = default;
	~GameServerConnectionManager() = default;

	__CLASS_NON_COPYABLE(GameServerConnectionManager)

public:
	void initialize();
	void connect();
	void disconnect();
	void finalize();

	void update();

public:
	void on_connection_succeeded();

public:
	bool is_established() const;
	bool is_connected() const;

	asio::ip::tcp::socket& get_socket();
	const asio::ip::tcp::socket& get_socket() const;

private:
	void on_connect_handler(const asio::error_code& errorCode);

private:
	void attach_context_thread();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE


private:
	asio::io_context context;
	asio::ip::tcp::socket socket{ context, asio::ip::tcp::v4() };
	ConnectionState connectionState{ ConnectionState::Disconnected };
	asio::steady_timer timer{ context };
	asio::ip::tcp::endpoint endpoint{};

	mutable std::mutex mutex;
	bool isThreadEnded = true;
	std::thread contextThread;
};
