#include "IActionBasic.h"

#include "IEntity.h"

#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationPlayer.h>

void IActionBasic::reset_animation() {
	NodeAnimationPlayer* animation = owner->get_animation();
	animation->reset_animation(useAnimationName);
	animation->restart();
	animation->set_loop(loopAnimation);
}

void IActionBasic::start(Reference<IEntity> owner_, const std::string& animationName) {
	owner = owner_;
	useAnimationName = animationName;
}

void IActionBasic::begin() {
	timer.ahead();
}

#ifdef DEBUG_FEATURES_ENABLE

void IActionBasic::debug_gui() {
}
#endif // DEBUG_FEATURES_ENABLE
