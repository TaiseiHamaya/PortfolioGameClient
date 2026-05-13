#include "ZoneSyncMessageHandler.h"

#include <chrono>

#include <Engine/Application/Logger.h>

#include <Library/Math/Vector3.h>

#include <Scripts/Proto/process/gateway/packet.pb.h>

#include "Scripts/Game/Zone/Command/ZoneEntityDamagedCommand.h"
#include "Scripts/Game/Zone/Command/ZonePlayActionCommand.h"
#include "Scripts/Game/Zone/Command/ZoneSyncTransformCommand.h"
#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"

void ZoneSyncMessageHandler::setup(
	Reference<EntityManager> entityManager_,
	Reference<GameLogWindowManager> gameLogWindowManager_,
	CommandStack commandStack_) {
	entityManager = entityManager_;
	gameLogWindowManager = gameLogWindowManager_;
	commandStack = std::move(commandStack_);
}

void ZoneSyncMessageHandler::set_player(Reference<Player> player_) {
	player = player_;
}

void ZoneSyncMessageHandler::operator()(const Proto::ToClientMessage& packet) {
	switch (packet.message_case()) {
	case Proto::ToClientMessage::kTransformSync: // 位置同期
	{
		const Proto::PayloadTransformSync& body = packet.transform_sync();
		if (player->server_id().has_value() && player->server_id().value() == body.id()) {
			// 自分自身の位置同期は処理しない
			break;
		}
		Vector3 position(body.position().x(), body.position().y(), body.position().z());
		using time_point = std::chrono::system_clock::time_point;
		time_point time{ std::chrono::duration_cast<time_point::duration>(std::chrono::microseconds(body.timestamp())) };

		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		commandStack(std::make_unique<ZoneSyncTransformCommand>(entity, time, position));
	}
	break;
	case Proto::ToClientMessage::kPlayAction: // アクション実行
	{
		const Proto::PayloadPlayAction& body = packet.play_action();
		Reference<IEntity> entity = entityManager->inquire_server_id(body.id());
		u64 targetId = body.target_id();
		Reference<IEntity> target = entityManager->inquire_server_id(targetId);
		if (entity && target) {
			szgInformation("Entity Id:{} Play Action Id:{}", body.id(), body.action_id());
			szgInformation("{} used action {} on {}.", entity->name_imm(), body.action_id(), target->name_imm());
			gameLogWindowManager->add_log(
				GameLogWindowManager::Type::ActionLog,
				ConvertString(std::format("{} action {}.", entity->name_imm(), body.action_id()))
			);
		}
		// 自分自身のアクションは処理しない
		if (player->server_id().has_value() && body.id() == player->server_id().value()) {
			break;
		}
		commandStack(std::make_unique<ZonePlayActionCommand>(entity, body.action_id(), target));
	}
	break;
	case Proto::ToClientMessage::kEntityDamaged: // ダメージ同期
	{
		const Proto::PayloadEntityDamaged& body = packet.entity_damaged();
		Reference<IEntity> target = entityManager->inquire_server_id(body.entity_id());
		commandStack(std::make_unique<ZoneEntityDamagedCommand>(target, body.damage()));
		if (target) {
			szgInformation("Entity {} took {} damage.", target->name_imm(), body.damage());
			gameLogWindowManager->add_log(
				GameLogWindowManager::Type::DamagedLog,
				ConvertString(std::format("{} damage to {}.", body.damage(), target->name_imm()))
			);
		}
	}
	break;
	default:
		break;
	}
}

