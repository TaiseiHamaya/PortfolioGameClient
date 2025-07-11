#pragma once

#include "Scripts/IEntity/IEntity.h"
#include "Scripts/IEntity/ISkillAction/ISkillAction.h"

class Player final : public IEntity {
public:
	void initialize(const std::filesystem::path& file) override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG


public:
	void set_target(Reference<IEntity> entity);
};
