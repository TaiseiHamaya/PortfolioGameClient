#include "TitleStateScript.h"

#include <Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h>
#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>
#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Input/InputTextFrame.h>
#include <Engine/Runtime/Input/InputVKeyState.h>
#include <Engine/Runtime/Scene/SceneManager2.h>

#include <Library/Math/Definition.h>

void TitleStateScript::setup() {
	selectingRect = std::any_cast<Reference<szg::Rect3d>>(*szg::RuntimeStorage::GetValueMut("RuntimeInstance", "Selecting"));
	loginString = std::any_cast<Reference<szg::StringRectInstance>>(*szg::RuntimeStorage::GetValueMut("RuntimeInstance", "Login"));
	nameString = std::any_cast<Reference<szg::StringRectInstance>>(*szg::RuntimeStorage::GetValueMut("RuntimeInstance", "Name"));
	inputString = std::any_cast<Reference<szg::StringRectInstance>>(*szg::RuntimeStorage::GetValueMut("RuntimeInstance", "InputtingString"));

	inputKey.initialize({ szg::KeyID::Space, szg::KeyID::Return, szg::KeyID::W, szg::KeyID::S, szg::KeyID::DownArrow, szg::KeyID::UpArrow });
	inputPad.initialize({ szg::PadID::A,szg::PadID::Up,szg::PadID::Down });
}

void TitleStateScript::finalize() {
}

void TitleStateScript::prev_update() {
	timer.ahead();
	inputKey.update();
	inputPad.update();

	// 上入力で上へ
	if (!isInputText) {
		if (
			inputKey.trigger(szg::KeyID::UpArrow) ||
			inputKey.trigger(szg::KeyID::W) ||
			szg::Input::StickL().y > 0.5f ||
			inputPad.trigger(szg::PadID::Up)
			) {
			selectIndex -= 1;
		}
		else if (
			inputKey.trigger(szg::KeyID::DownArrow) ||
			inputKey.trigger(szg::KeyID::S) ||
			inputPad.trigger(szg::PadID::Down) ||
			szg::Input::StickL().y < -0.5f
			) {
			selectIndex += 1;
		}
		selectIndex = std::clamp(selectIndex, 0, 1);
	}

	switch (selectIndex) {
	case 0:
		if (inputKey.trigger(szg::KeyID::Space) || inputKey.trigger(szg::KeyID::Return) || inputPad.trigger(szg::PadID::A)) {
			isInputText ^= 1;
		}
		break;
	case 1:
		if (inputString->glyph_data_imm().empty()) {
			break;
		}
		if (inputKey.trigger(szg::KeyID::Return) || inputPad.trigger(szg::PadID::A)) {
			if (state == State::Title) {
				szg::RuntimeStorage::GetValueList("Temp")["PlayerName"] = inputString->string_imm();
				szg::SceneManager2::SceneChange(0, 0.5f);
				state = State::Login;
			}
		}
		break;
	default:
		break;
	}


	if (inputString->glyph_data_imm().empty()) {
		loginString->get_material().color = ColorRGBA(0x202020ff);
		nameString->set_draw(true);
		inputString->set_draw(false);
	}
	else {
		loginString->get_material().color = CColorRGBA::WHITE;
		nameString->set_draw(false);
		inputString->set_draw(true);
	}

	if (isInputText) {
		if (szg::InputVKeyState::IsDownVKey(szg::VirtualKeyID::Backspace)) {
			inputString->pop_back();
		}
		const std::wstring& inputChars = szg::InputTextFrame::FrameInputTextImm();
		std::wstring appendStr;
		for (wchar_t c : inputChars) {
			if (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && inputString->glyph_data_imm().size() + appendStr.size() < 12) {
				appendStr += c;
			}
		}
		if (!appendStr.empty()) {
			inputString->append(appendStr);
		}
	}

	selectingRect->transform_mut().set_translate_y(
		std::vector{ nameString->transform_mut().get_translate().y, loginString->transform_mut().get_translate().y } [selectIndex]
	);
	if (isInputText) {
		selectingRect->get_material().color.alpha = 0.5f;
		nameString->get_material().color.alpha = 0.5f;
	}
	else {
		selectingRect->get_material().color.alpha = std::sin(timer * PI2);
		nameString->get_material().color.alpha = 1.0f;
	}
}

void TitleStateScript::post_update() {
}
