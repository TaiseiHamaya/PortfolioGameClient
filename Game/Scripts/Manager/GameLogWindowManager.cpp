#include "GameLogWindowManager.h"

#include <Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h>
#include <Engine/Module/Manager/World/WorldRoot.h>
#include <Engine/Runtime/Input/Input.h>

void GameLogWindowManager::initialize() {
	auto logTextRoot = szg::RuntimeStorage::GetValueMut("RuntimeInstance", "LogTextRoot");
	Reference<szg::WorldInstance> chatTextRoot =
		std::any_cast<Reference<szg::WorldInstance>>(
			*logTextRoot
		);
	if (chatTextRoot) {
		// ログ表示用のStringRectInstanceを作成
		Reference<szg::WorldRoot> world = chatTextRoot->world_root_mut();
		r32 lineHeight = 34;
		for (i32 i = 0; i < MaxLogCount; ++i) {
			Reference<szg::StringRectInstance> instance = world->instantiate<szg::StringRectInstance>(chatTextRoot);
			instance->initialize("UDEVGothic35HS-Regular.mtsdf", 550, CVector2::ZERO);
			instance->transform_mut().set_translate_y(lineHeight * (MaxLogCount - i - 1) + 60);
			instance->set_draw(false);
			logInstances[i] = instance;
		}
	}
}

void GameLogWindowManager::prev_update() {
	// スクロール処理
	r32 wheel = szg::Input::WheelDelta();
	if (wheel) {
		index -= static_cast<i32>(std::copysign(1.0f, wheel)); // 1行動かす
		// 範囲外に出ないようにする
		index = std::clamp(index, 0, std::max(0, static_cast<i32>(logs.size()) - MaxLogCount));
		isChanged = true;
	}

	if (isChanged) {
		update_log_window();
	}
}

void GameLogWindowManager::post_update() {
}

void GameLogWindowManager::add_log(Type type, const std::wstring& log) {
	logs.emplace_back(type, log);
	isChanged = true;
	// 一番下だったらスクロールを維持
	if (index + 1 + MaxLogCount >= static_cast<i32>(logs.size())) {
		index = std::max(0, static_cast<i32>(logs.size()) - MaxLogCount);
	}
}

void GameLogWindowManager::update_log_window() {
	// ログ表示更新
	// 最大MaxLogCount件表示
	for (i32 li = 0; li < MaxLogCount; ++li) {
		i32 refIndex = index + li;
		if (refIndex >= static_cast<i32>(logs.size())) {
			break;
		}

		Reference<szg::StringRectInstance> instance = logInstances[li];
		if (!instance) {
			continue;
		}
		const LogBody& logBody = logs[refIndex];
		instance->reset_string(logBody.message);
		instance->get_material().color = typeColors[static_cast<i32>(logBody.type)];
		instance->set_draw(true);
	}
	isChanged = false;
}
