#pragma once

#include "Scripts/Instance/IEntity/IEntity.h"
#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"

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
