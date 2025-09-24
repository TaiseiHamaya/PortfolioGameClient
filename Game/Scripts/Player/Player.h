#pragma once

#include "Scripts/IEntity/IEntity.h"
#include "Scripts/IEntity/ISkillAction/ISkillAction.h"

class Player final : public IEntity {
public:
	void initialize(const std::filesystem::path& file) override;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE


public:
	void set_target(Reference<IEntity> entity);
};
