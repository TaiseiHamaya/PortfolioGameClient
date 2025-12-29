#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>
#include <Engine/Runtime/Input/InputHandler.h>
#include <Engine/Runtime/Input/InputTextController.h>

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

	std::wstring into_string();

private:
	bool isSendFrame{ false };
	szg::InputHandler<szg::KeyID> keys;
	Reference<szg::StringRectInstance> chatBoxString;
	Reference<szg::Rect3d> chatBoxCursor;
	szg::InputTextController textBox;
};
