#include "GameInputHandler.h"

#include <Engine/Runtime/Clock/WorldClock.h>

#include "Scripts/Game/Zone/ZoneHandler.h"
#include "Scripts/Instance/Player/Player.h"

#include <Library/Utility/Tools/RandomEngine.h>

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Application/ArgumentParser.h>
#endif

void GameInputHandler::initialize() {
	inputKey = {
		{szg::KeyID::Space, szg::PadID::Y}, // ジャンプ
		{szg::KeyID::One, szg::PadID::A}, // アクション1
	};

	std::vector<szg::KeyID> keys;
	std::vector<szg::PadID> pads;
	// 配列に並べ替え
	for (auto& [key, pad] : inputKey) {
		keys.emplace_back(key);
		pads.emplace_back(pad);
	}
	// 入力の登録
	actionHandlerK.initialize(std::move(keys));
	actionHandlerP.initialize(std::move(pads));

#ifdef DEBUG_FEATURES_ENABLE
	timer.set(RandomEngine::Random01MOD() + BOT_MOVE_TIME);
#endif // DEBUG_FEATURES_ENABLE
};

void GameInputHandler::setup(Reference<ZoneHandler> zoneHandler_) {
	zoneHandler = zoneHandler_;
}

void GameInputHandler::prev_update() {
	input();
	update();
}

void GameInputHandler::post_update() {
}

void GameInputHandler::input() {
	// キー入力
	actionHandlerK.update();
	actionHandlerP.update();
	inputDirection = szg::InputAdvanced::PressWASD();
	if (inputDirection.length() == 0) {
		inputDirection = szg::Input::StickL();
	}
}

void GameInputHandler::update() {
	if (!player) {
		return;
	}

#ifdef DEBUG_FEATURES_ENABLE
	// 開発用自動移動ボット
	if (szg::ArgumentParser::Contains("--develop-bot-enable")) {
		timer.ahead();
		Vector3 center;
		// 中心座標をコマンドライン引数から取得
		std::optional<u64> xArgIndex = szg::ArgumentParser::ArgIndexByValue("-x");
		std::optional<u64> yArgIndex = szg::ArgumentParser::ArgIndexByValue("-y");
		if (xArgIndex) {
			center.x = std::stof(szg::ArgumentParser::ValueByIndex(xArgIndex.value() + 1));
		}
		if (yArgIndex) {
			center.z = std::stof(szg::ArgumentParser::ValueByIndex(yArgIndex.value() + 1));
		}
		if (timer >= BOT_MOVE_TIME) {
			timer.set(timer - BOT_MOVE_TIME);
		}
		// 2piを3秒で一周
		r32 angle = (timer / BOT_MOVE_TIME) * PI2;
		constexpr r32 RADIUS = 2.0f;
		Vector3 angleVector = CVector3::FORWARD * RADIUS * Quaternion::AngleAxis(CVector3::BASIS_Y, angle);

		// ゾーンに移動要求
		zoneHandler->move_client_player(center + angleVector);
		return;
	}
#endif // DEBUG_FEATURES_ENABLE

	if (inputDirection.length() > 0) {
		Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
		Vector2 xzForward = { -cameraForward.x, cameraForward.z };
		// 正面
		xzForward = xzForward.normalize();
		// 入力方向をカメラ基準に変換
		xzDirection = Vector2::Rotate(inputDirection, xzForward.x, xzForward.y);
		Vector3 xzDirection3 = { xzDirection.x, 0.0f, xzDirection.y };

		const Vector3& position = player->get_transform().get_translate();

		Vector3 velocity = xzDirection3 * player->get_move_speed();
		Vector3 dest = position + velocity * szg::WorldClock::DeltaSeconds();

		zoneHandler->move_client_player(dest);
	}

	// アクション実行
	if (actionHandlerK.trigger(inputKey[1].first) || actionHandlerP.trigger(inputKey[1].second)) {
		zoneHandler->request_play_action(0);
	}
}

void GameInputHandler::set_instances(Reference<Player> player_, Reference<const szg::WorldInstance> camera_) {
	player = player_;
	camera = camera_;
}
