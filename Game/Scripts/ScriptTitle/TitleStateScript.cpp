#include "TitleStateScript.h"

#include <Engine/Runtime/BackgroundLoader/BackgroundLoader.h>
#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Runtime/RuntimeStorage/RuntimeStorage.h>
#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>
#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Input/InputTextFrame.h>
#include <Engine/Runtime/Input/InputVKeyState.h>
#include <Engine/Runtime/Scene/SceneManager2.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Template/Reference.h>

#include "Scripts/Proto/process/gateway/packet.pb.h"

#include "Scripts/Network/NetworkCluster.h"
#include "Scripts/Scene/FactoryPortfolio.h"

void TitleStateScript::setup() {
	selectingRect = szg::RuntimeStorage::GetValue<Reference<szg::Rect3d>>("RuntimeInstance", "Selecting").value();
	loginString = szg::RuntimeStorage::GetValue<Reference<szg::StringRectInstance>>("RuntimeInstance", "Login").value();
	nameString = szg::RuntimeStorage::GetValue<Reference<szg::StringRectInstance>>("RuntimeInstance", "Name").value();
	inputString = szg::RuntimeStorage::GetValue<Reference<szg::StringRectInstance>>("RuntimeInstance", "InputtingString").value();
	errorMessageString = szg::RuntimeStorage::GetValue<Reference<szg::StringRectInstance>>("RuntimeInstance", "ErrorMessage").value();
	Reference<szg::Rect3d> rect = szg::RuntimeStorage::GetValue<Reference<szg::Rect3d>>("RuntimeInstance", "LoadingIcon").value();
	waitUntil.initialize(rect);

	inputKey.initialize({ szg::KeyID::Space, szg::KeyID::Return, szg::KeyID::W, szg::KeyID::S, szg::KeyID::DownArrow, szg::KeyID::UpArrow });
	inputPad.initialize({ szg::PadID::A,szg::PadID::Up,szg::PadID::Down });

	state = State::Login;
}

void TitleStateScript::finalize() {
}

void TitleStateScript::prev_update() {
	NetworkCluster::Receive();

	switch (state) {
	case TitleStateScript::State::None:
		update_none();
		break;
	case TitleStateScript::State::Signup:
		update_signup();
		break;
	case TitleStateScript::State::Login:
		update_login();
		break;
	case TitleStateScript::State::Lobby:
		update_lobby();
		break;
	case TitleStateScript::State::Loading:
		update_loading();
		break;
	default:
		break;
	}

	if (isInputText) {
		selectingRect->get_material().color.alpha = 0.5f;
		nameString->get_material().color.alpha = 0.5f;
	}
	else {
		selectingRect->get_material().color.alpha = std::sin(timer * PI2);
		nameString->get_material().color.alpha = 1.0f;
	}

	selectingRect->transform_mut().set_translate_y(
		std::vector{ nameString->transform_mut().get_translate().y, loginString->transform_mut().get_translate().y } [selectIndex]
	);
}

void TitleStateScript::post_update() {
	NetworkCluster::Send();
}

void TitleStateScript::update_none() {
	// なにかおかしいのでゲームを強制終了
	szg::SceneManager2::PopScene(1.0f);
	errorMessageString->reset_string("An unexpected error has occurred. Please restart the game.");
}

void TitleStateScript::update_signup() {
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

	switch (static_cast<SelectIndex>(selectIndex)) {
	case SelectIndex::Login:
		if (inputKey.trigger(szg::KeyID::Space) || inputKey.trigger(szg::KeyID::Return) || inputPad.trigger(szg::PadID::A)) {
			isInputText ^= 1;
		}
		break;
	case SelectIndex::StartGame:
		if (inputString->glyph_data_imm().empty()) {
			break;
		}
		if (inputKey.trigger(szg::KeyID::Return) || inputPad.trigger(szg::PadID::A)) {
			state = State::Loading;

			// メッセージ送信
			Proto::ToServerMessage packet;
			Proto::PayloadSignupRequest* signupRequest = packet.mutable_signup_request();
			signupRequest->set_username(inputString->string_imm());
			NetworkCluster::SenderMut()->stack_packet(packet);
			szgInformation("Signup request sent. Username: {}", inputString->string_imm());

			waitUntil.reset([&]() -> bool {
				// メッセージ受け取り
				auto messages = NetworkCluster::ReceiverMut()->take_packet_stack();

				for (auto message : messages) {
					if (message.message_case() == Proto::ToClientMessage::kSignupResponse) {
						const Proto::PayloadSignupResponse& signupResponse = message.signup_response();
						// 成功時
						if (signupResponse.is_succeeded()) {
							state = State::Login;
							// SessionIDをローカルに保存
							const Proto::SessionId& sessionId = signupResponse.session_id();
							szg::JsonAsset sessionCache{ "[[game]]/SessionCache.json" };
							sessionCache.get()["SessionId"] = nlohmann::json::object();
							sessionCache.get()["SessionId"]["High"] = sessionId.high();
							sessionCache.get()["SessionId"]["Low"] = sessionId.low();

							sessionCache.save();
						}
						// 失敗時
						else {
							state = State::Signup;
							errorMessageString->reset_string("Signup failed. Please try again.");
						}
						return true;
					}
				}

				// タイムアウト
				if (waitUntil.timer_imm() >= 10.0f) {
					state = State::Signup;
					errorMessageString->reset_string("Failed to connect to server. Please try again.");
					return true;
				}
				return false;
			});
			onLoaded = [&]() {
			};
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
}

void TitleStateScript::update_login() {
	szg::JsonAsset sessionCache{ "[[game]]/SessionCache.json" };
	if (sessionCache.cget().is_null() || !sessionCache.get().contains("SessionId")) {
		state = State::Signup;
	}
	else {
		auto sessionIdObj = sessionCache.get().value("SessionId", nlohmann::json::object());
		state = State::Loading;

		// メッセージ送信
		Proto::ToServerMessage packet;
		Proto::PayloadLoginRequest* loginRequest = packet.mutable_login_request();
		Proto::SessionId* sessionId = loginRequest->mutable_session_id();
		sessionId->set_high(sessionIdObj.value("High", 0ull));
		sessionId->set_low(sessionIdObj.value("Low", 0ull));
		NetworkCluster::SenderMut()->stack_packet(packet);

		waitUntil.reset([&]() -> bool {
			// メッセージ受け取り
			auto messages = NetworkCluster::ReceiverMut()->take_packet_stack();

			for (auto message : messages) {
				if (message.message_case() == Proto::ToClientMessage::kLobbyEnterResponse) {
					const Proto::PayloadLobbyEnterResponse& lobbyEnterResponse = message.lobby_enter_response();
					// 成功時
					if (lobbyEnterResponse.is_succeeded()) {
						// ロビーへ遷移
						state = State::Lobby;
						nameString->reset_string(lobbyEnterResponse.character_name());
						loginString->reset_string("Game start");
						errorMessageString->reset_string("");
					}
					// 失敗時
					else {
						// セッションが無効なので、キャラクター作成をやり直す
						state = State::Signup;
						loginString->reset_string("Signup");
						errorMessageString->reset_string("Session expired or invalid. Please signup again.");
					}
					return true;
				}
			}

			// タイムアウト
			if (waitUntil.timer_imm() >= 10.0f) {
				state = State::Signup;
				errorMessageString->reset_string("Failed to connect to server. Please try again.");
				return true;
			}

			return false;
		});
		onLoaded = [&]() {
		};
	}
}

void TitleStateScript::update_lobby() {
	timer.ahead();
	inputKey.update();
	inputPad.update();

	selectIndex = 1;

	if (inputKey.trigger(szg::KeyID::Return) || inputPad.trigger(szg::PadID::A)) {
		state = State::Loading;

		Proto::ToServerMessage enter;
		Proto::PayloadLobbyStartGameRequest* payload = enter.mutable_start_game();
		NetworkCluster::SenderMut()->stack_packet(enter);

		NetworkCluster::Send();
		// シーン遷移の開始
		szg::SceneManager2::SceneChange(SceneListPortfolio::SCENE_GAME, std::numeric_limits<r32>::max());
		waitUntil.reset([&]() -> bool {
			// ゲームのロードが終わるまで待つ
			return !szg::BackgroundLoader::IsLoading();
		});
		onLoaded = [&]() {
			// ロードが終わったら移動を開始
			szg::SceneManager2::EndSceneChangeIntervalForce();
		};
		szg::RuntimeStorage::GetValueList("Temp")["PlayerName"] = nameString->string_imm();
	}
}

void TitleStateScript::update_loading() {
	if (waitUntil.update()) {
		if (onLoaded) {
			onLoaded();
			onLoaded = nullptr;
		}
	}
}
