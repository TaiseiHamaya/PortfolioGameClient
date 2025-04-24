#include "BaseAction.h"

#include "BaseEntity.h"

#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationPlayer.h>

void BaseAction::reset_animation() {
	NodeAnimationPlayer* animation = owner->get_animation();
	animation->reset_animation(useAnimationName);
	animation->restart();
	animation->set_loop(loopAnimation);
}

void BaseAction::start(Reference<BaseEntity> owner_, const std::string& animationName) {
	owner = owner_;
	useAnimationName = animationName;
}

void BaseAction::begin() {
	timer.ahead();
}

#ifdef DEBUG_FEATURES_ENABLE

void BaseAction::debug_gui() {
}
#endif // DEBUG_FEATURES_ENABLE
