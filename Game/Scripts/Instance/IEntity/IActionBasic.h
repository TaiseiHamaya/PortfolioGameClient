#pragma once

#include <Engine/Runtime/Clock/WorldTimer.h>

#include <Library/Utility/Template/Reference.h>

class IEntity;

#include <string>

enum class ActionEffect {
	None,
	Spell,
	Stack
};

enum class ActionType {
	Unknown,
	WeaponSkill,
	Spell,
	Ability,
	Misc,
};

/// <summary>
/// アクション用インターフェース
/// </summary>
class IActionBasic {
public:
	IActionBasic() = default;
	virtual ~IActionBasic() = default;

public:
	virtual void setup(Reference<IEntity> owner_, const std::string& animationName);
	virtual void start() = 0;
	virtual void update() = 0;

	virtual void reset() = 0;
	void reset_animation();

public:
	virtual bool can_transition() const = 0;
	virtual bool end_action() const = 0;

public:
	ActionEffect action_effect() const { return effect; }
	ActionType action_type() const noexcept;

#ifdef DEBUG_FEATURES_ENABLE
public:
	virtual void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

protected:
	szg::WorldTimer timer;
	ActionEffect effect{ ActionEffect::None };
	ActionType type{ ActionType::Unknown };
	Reference<IEntity> owner{ nullptr };
	std::string useAnimationName{ "Armatureアクション" };
	bool loopAnimation{ false };
};
