#pragma once

#include <Engine/Runtime/Clock/WorldTimer.h>

#include <Library/Utility/Template/Reference.h>

class IEntity;

#include <string>

enum class ActionType {
	WeaponSkill,
	Spell,
	Ability
};
enum class ActionEffect {
	Nane,
	Spell,
	Stack
};

class IActionBasic {
public:
	IActionBasic() = default;
	virtual ~IActionBasic() = default;

public:
	void start(Reference<IEntity> owner_, const std::string& animationName);
	void begin();
	virtual void update() = 0;

	virtual void reset() = 0;
	void reset_animation();

public:
	virtual r32 progress() const = 0;
	virtual bool can_transition() const = 0;
	virtual bool end_action() const = 0;

public:
	ActionType action_type() const { return type; }
	ActionEffect action_effect() const { return effect; }

#ifdef _DEBUG
public:
	virtual void debug_gui();
#endif // _DEBUG

protected:
	WorldTimer timer;

	float CastTime{ 0 };
	float RecastTime{ 2.5f };
	float Range{ 0 };
	float Radius{ 0 };

	ActionType type;
	ActionEffect effect;
	Reference<IEntity> owner{ nullptr };
	std::string useAnimationName{ "Armatureアクション" };
	bool loopAnimation{ false };
};
