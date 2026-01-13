#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>
#include <Engine/Runtime/Clock/WorldTimer.h>
#include <Engine/Runtime/Input/InputHandler.h>

#include <Library/Utility/Template/Reference.h>

namespace szg {
class StringRectInstance;
class Rect3d;
};

class TitleStateScript final : public szg::ISceneScript {
public:
	enum class State {
		Title,
		Login,
		Transition,
	};

	enum class SelectIndex {
		Login,
		StartGame,
	};

public:
	TitleStateScript() = default;
	~TitleStateScript() noexcept = default;

	SZG_CLASS_MOVE_ONLY(TitleStateScript)

public:
	void setup();

	void finalize() override;

	void prev_update() override;
	void post_update() override;

private:
	szg::WorldTimer timer;

	State state{ State::Title };
	bool isInputText;
	i32 selectIndex{ 0 };

	Reference<szg::StringRectInstance> loginString;
	Reference<szg::StringRectInstance> nameString;
	Reference<szg::StringRectInstance> inputString;
	Reference<szg::Rect3d> selectingRect;

	szg::InputHandler<szg::KeyID> inputKey;
	szg::InputHandler<szg::PadID> inputPad;
};

