#include "ChatBoxManager.h"

#include <Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h>
#include <Engine/Runtime/Input/InputTextFrame.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Template/Reference.h>

void ChatBoxManager::initialize() {
	chatBoxString = szg::RuntimeStorage::GetValue<Reference<szg::StringRectInstance>>("RuntimeInstance", "InputtingText").value();
	chatBoxCursor = szg::RuntimeStorage::GetValue<Reference<szg::Rect3d>>("RuntimeInstance", "ChatBoxCursor").value();

	// トリガーとなるキーの登録
	keys.initialize({ szg::KeyID::Return, szg::KeyID::Escape });
}

void ChatBoxManager::update() {
	keys.update();
	cursorTimer.ahead();
	isSendFrame = false;

	// 切り替え
	if (textBox.is_inputting()) {
		// Enter + IME確定ではない場合、送信
		if (keys.trigger(szg::KeyID::Return) && !szg::InputTextFrame::IsImmEnter()) {
			textBox.end_input();
			const std::wstring& text = textBox.text_imm();
			if (!text.empty()) { // 空文字列出ない場合のみ
				isSendFrame = true;
			}
		}
		// ESCキーで入力解除
		else if (keys.trigger(szg::KeyID::Escape)) {
			textBox.end_input();
		}

		// 入力終了処理
		if (!textBox.is_inputting()) {
			// カーソルの非表示
			if (chatBoxCursor) {
				chatBoxCursor->set_draw(false);
			}
		}
	}
	else {
		if (keys.trigger(szg::KeyID::Return)) {
			// 入力開始
			Vector2 position{ 0, 1080 };
			textBox.start_input(position);
			chatBoxCursor->set_draw(true);
			cursorTimer.set(0.0f);
		}
	}

	// テキストボックスの更新
	textBox.update();

	// チャットボックスの文字列更新
	const std::wstring& text = textBox.text_imm();
	if (chatBoxString) {
		chatBoxString->reset_string(text);
	}

	if (chatBoxCursor) {
		// カーソルの点滅処理
		chatBoxCursor->get_material().color.alpha = std::sin(cursorTimer * PI2);
	}

	// カーソル位置の更新
	if (chatBoxCursor && chatBoxString) {
		r32 offset =
			textBox.calculate_cursor_offset(chatBoxString);
		chatBoxCursor->transform_mut().set_translate_x(-offset - 12);
	}
}

bool ChatBoxManager::is_enter_frame() const {
	return isSendFrame;
}

bool ChatBoxManager::is_inputting() const {
	return textBox.is_inputting();
}

std::wstring ChatBoxManager::into_string() {
	std::wstring result = textBox.text_imm();
	textBox.clear();
	chatBoxString->reset_string(L"");

	return result;
}
