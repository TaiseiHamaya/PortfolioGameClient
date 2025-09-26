#pragma once

#include <asio.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

class GameServerConnectionManager final {
public:
	GameServerConnectionManager() = default;
	~GameServerConnectionManager() = default;

	__CLASS_NON_COPYABLE(GameServerConnectionManager)

public:
	void initialize();
	void connect();
	void disconnect();
	void finalize();

public:
	bool is_connected() const;

	asio::ip::tcp::socket& get_socket();
	const asio::ip::tcp::socket& get_socket() const;

private:
	void on_connect_handler(const asio::error_code& errorCode);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE


private:
	asio::io_context context;
	asio::strand<asio::io_context::executor_type> strand{ asio::make_strand(context) };
	asio::ip::tcp::socket socket{ context, asio::ip::tcp::v4() };
	bool isConnected{ false };
};
