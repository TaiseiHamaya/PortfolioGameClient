#pragma once

#include <functional>
#include <map>
#include <vector>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// 受信メッセージのルーター
/// </summary>
class ReceivedMessageRouter {
public:
	ReceivedMessageRouter() = default;
	~ReceivedMessageRouter() = default;

	SZG_CLASS_MOVE_ONLY(ReceivedMessageRouter)

public:
	using MessageHandler = std::function<void(const Proto::ToClientMessage&)>;

public:
	void initialize();

	/// <summary>
	/// ハンドラの登録
	/// </summary>
	void register_handler(Proto::ToClientMessage::MessageCase messageCase, MessageHandler handler);

	/// <summary>
	/// パケットの振り分け
	/// </summary>
	void dispatch(const std::vector<Proto::ToClientMessage>& packets);

private:
	std::map<Proto::ToClientMessage::MessageCase, MessageHandler> handlers;
};
