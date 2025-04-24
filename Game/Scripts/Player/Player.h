#pragma once

#include "Scripts/IEntity/BaseEntity.h"

class Player final : public BaseEntity {
#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:

};
