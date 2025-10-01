#include "ZoneHandler.h"

#include "./GameServer/GameServerConnectionManager.h"
#include "./GameServer/GameServerPacketReceiver.h"

#include <Engine/Application/Output.h>

void ZoneHandler::setup(Reference<GameServerPacketReceiver> gameServerPacketReceiver_, Reference<GameServerConnectionManager> gameServerConnectionManager_) {
	gameServerPacketReceiver = gameServerPacketReceiver_;
	gameServerConnectionManager = gameServerConnectionManager_;
}

void ZoneHandler::handle_zone() {
	if (!gameServerPacketReceiver) {
		return;
	}

	auto packets = gameServerPacketReceiver->take_packet_stack();

	for (auto& packet : packets) {
		switch (packet.category_case()) {
		case Proto::Packet::kTextMessageType:
			// TextMessageTypeの処理
			process_text_message(packet.textmessagetype(), packet.payload());
			break;
		case Proto::Packet::kLoginPacketType:
			// LoginPacketTypeの処理
			process_login_packet(packet.loginpackettype(), packet.payload());
			break;
		case Proto::Packet::kLogoutPacketType:
			// LogoutPacketTypeの処理
			break;
		case Proto::Packet::kSyncPacketType:
			// SyncPacketTypeの処理
			break;
		default:
			Warning("");
			// その他の処理
			break;
		}
	}
}

void ZoneHandler::process_text_message(Proto::TextMessageType type, const std::string& payload) {
	switch (type) {
	case Proto::MessageChatReceive:
	{
		Proto::ChatMessageBody body;
		body.ParseFromString(payload);
		Information("[{}]: {}", body.userid(), body.message());
	}
	break;
	case Proto::MessageSystemMessage:
	{
		Proto::SystemMessageBody body;
		body.ParseFromString(payload);
		Information("[System]: {}", body.message());
	}
	break;
	}
}

void ZoneHandler::process_login_packet(Proto::LoginPacketType type, const std::string& payload) {
	switch (type) {
	case Proto::LoginResult:
	{
		Proto::LoginResultBody body;
		body.ParseFromString(payload);
		gameServerConnectionManager->on_connection_succussed();

	}
	break;
	case Proto::LoginNotification:
	{
		Proto::LoginNotificationBody body;
		body.ParseFromString(payload);
		Information("[System]: Player added. Id-\'{}\' Name-\'{}\'", body.userid(), body.username());
	}
	break;
	default:
		break;
	}
}
