#pragma once

#include <thread>

#include <asio.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// Zoneサーバーとの接続を管理するクラス
/// </summary>
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

	// mutexがあるため、コピー/ムーブ禁止
	__CLASS_NON_COPYMOVEABLE(GameServerConnectionManager)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize();

	/// <summary>
	/// 接続要求
	/// </summary>
	void connect();

	/// <summary>
	/// 切断要求
	/// </summary>
	void disconnect();

	/// <summary>
	/// 終了処理
	/// </summary>
	void finalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

public:
	/// <summary>
	/// 接続完了時に呼ぶ
	/// </summary>
	void on_connection_succeeded();

public:
	/// <summary>
	/// Socket開通済みかどうか
	/// </summary>
	/// <returns></returns>
	bool is_established() const;

	/// <summary>
	/// アプリケーションレベルで接続完了しているかどうか
	/// </summary>
	/// <returns></returns>
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
