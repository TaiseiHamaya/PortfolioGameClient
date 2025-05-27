#pragma once

#include "Scripts/IEntity/IEntity.h"
#include "Scripts/IEntity/ISkillAction/ISkillAction.h"

class Player final : public IEntity {
#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	std::vector<std::unique_ptr<ISkillAction>> skillActions;
};
