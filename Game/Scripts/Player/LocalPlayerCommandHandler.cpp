#include "LocalPlayerCommandHandler.h"

#include "Scripts/EntityCommand/Command/MoveCommand.h"
#include "Scripts/EntityCommand/Command/JumpCommand.h"

void LocalPlayerCommandHandler::start(Reference<WorldInstance> camera_) {
	commands.emplace_back(std::make_unique<MoveCommand>(xzDirection));
	commands.emplace_back(std::make_unique<JumpCommand>());

	actionTriggers.emplace_back(KeyID::Space, PadID::Y);

	std::vector<KeyID> keys;
	std::vector<PadID> pads;
	for (auto& actionTrigger : actionTriggers) {
		keys.emplace_back(actionTrigger.key);
		pads.emplace_back(actionTrigger.pad);
	}
	actionHandlerK.initialize(std::move(keys));
	actionHandlerP.initialize(std::move(pads));

	camera = camera_;
}

void LocalPlayerCommandHandler::begin() {
	actionHandlerK.update();
	actionHandlerP.update();
	// キー入力
	inputDirection = InputAdvanced::PressWASD();
	if (inputDirection.length() == 0) {
		inputDirection = Input::StickL();
	}
}

void LocalPlayerCommandHandler::update() {
	if (inputDirection.length() > 0) {
		Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
		Vector2 xzForward = { -cameraForward.x, cameraForward.z };
		xzForward = xzForward.normalize();
		xzDirection = Vector2::Rotate(inputDirection, xzForward.x, xzForward.y);
		entry_command(0);
	}

	if (receiver->now_action()->progress() >= 0.8f && !actionBuffer.has_value()) {
		for (u32 i = NumPreAction; ActionKeys& trigger : actionTriggers) {
			if (actionHandlerK.trigger(trigger.key) || actionHandlerP.trigger(trigger.pad)) {
				actionBuffer = i;
				break;
			}
			++i;
		}
	}

	// 遷移可能な場合は遷移
	if (receiver->now_action()->can_transition() && actionBuffer.has_value()) {
		// コマンド発行
		entry_command(actionBuffer.value());
		// 先行入力を削除
		actionBuffer = std::nullopt;
	}
}
