#include "ZoneTextMessageHandler.h"

#include <Engine/Application/Logger.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"

void ZoneTextMessageHandler::setup(Reference<EntityManager> entityManager_, Reference<GameLogWindowManager> gameLogWindowManager_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
}

void ZoneTextMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	case Proto::ToClientMessage::kTextMessage: // チャット受け取り
	{
		const Proto::PayloadTextMessage& body = packet.text_message();
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		if (entity) {
			szgInformation("[{}]: {}", entity->name_imm(), body.message());
			gameLogWindowManager->add_log(
				GameLogWindowManager::Type::ChatMessage,
				ConvertString(std::format("[{}]: {}", entity->name_imm(), body.message()))
			);
		}
		else {
			szgInformation("[Unknown]: {}", body.message());
			gameLogWindowManager->add_log(
				GameLogWindowManager::Type::ChatMessage,
				ConvertString(std::format("[Unknown]: {}", body.message()))
			);
		}
	}
	break;
	//case Proto::ToClientMessage::kSystemMessage: // サーバーメッセージ受け取り
	//{
	//	const Proto::PayloadSystemMessage& body = packet.system_message();
	//	szgInformation("[System]: {}", body.message());
	//	gameLogWindowManager->add_log(
	//		GameLogWindowManager::Type::SystemMessage,
	//		ConvertString(std::format("[System]: {}", body.message()))
	//	);
	//}
	//break;
	default:
		break;
	}
}
