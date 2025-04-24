#pragma once

#include "Scripts/EntityCommand/Hander/IEntityCommandHandler.h"

class EnemyRemoteHandler final : public IEntityCommandHandler {
public:
	inline static Reference<BaseEntity> player{ nullptr };
};
