#include "GameInputHandler.h"

#include <Engine/Runtime/Clock/WorldClock.h>

#include "Scripts/Game/Zone/ZoneCommand/ZoneHandler.h"
#include "Scripts/Instance/Player/Player.h"

void GameInputHandler::initialize() {
	inputKey = {
		{KeyID::Space, PadID::Y},
		{KeyID::One, PadID::A},
	};

	std::vector<KeyID> keys;
	std::vector<PadID> pads;
	for (auto& [key, pad] : inputKey) {
		keys.emplace_back(key);
		pads.emplace_back(pad);
	}
	actionHandlerK.initialize(std::move(keys));
	actionHandlerP.initialize(std::move(pads));
};

void GameInputHandler::setup(Reference<ZoneHandler> zoneHandler_) {
	zoneHandler = zoneHandler_;
}

void GameInputHandler::input() {
	actionHandlerK.update();
	actionHandlerP.update();
	// キー入力
	inputDirection = InputAdvanced::PressWASD();
	if (inputDirection.length() == 0) {
		inputDirection = Input::StickL();
	}
}

void GameInputHandler::update() {
	if (!player) {
		return;
	}

	if (inputDirection.length() > 0) {
		Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
		Vector2 xzForward = { -cameraForward.x, cameraForward.z };
		xzForward = xzForward.normalize();
		xzDirection = Vector2::Rotate(inputDirection, xzForward.x, xzForward.y);
		Vector3 xzDirection3 = { xzDirection.x, 0.0f, xzDirection.y };

		const Vector3& position = player->get_transform().get_translate();

		Vector3 velocity = xzDirection3 * 8.0f;
		Vector3 dest = position + velocity * WorldClock::DeltaSeconds();

		zoneHandler->move_client_player(dest);
	}
}

void GameInputHandler::set_instances(Reference<Player> player_, Reference<const WorldInstance> camera_) {
	player = player_;
	camera = camera_;
}
