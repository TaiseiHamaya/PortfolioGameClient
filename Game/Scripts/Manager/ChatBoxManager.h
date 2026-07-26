#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>
#include <Engine/Runtime/Input/InputHandler.h>
#include <Engine/Runtime/Input/InputTextController.h>

/// <summary>
/// チャットボックスマネージャ
/// プレイヤーのチャットメッセージ入力と表示を管理
/// </summary>
class ChatBoxManager final {
public:
	ChatBoxManager() = default;
	~ChatBoxManager() = default;

	SZG_CLASS_MOVE_ONLY(ChatBoxManager)

public:
	void initialize();

	void update();

public:
	bool is_enter_frame() const;

	bool is_inputting() const;

	std::wstring into_string();

private:
	bool isSendFrame{ false };
	szg::WorldTimer cursorTimer;
	szg::InputHandler<szg::KeyID> keys;
	Reference<szg::StringRectInstance> chatBoxString;
	Reference<szg::Rect3d> chatBoxCursor;
	szg::InputTextController textBox;
};
