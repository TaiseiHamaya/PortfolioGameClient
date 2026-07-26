#pragma once

#include <Engine/Assets/Json/JsonAsset.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

#include "../IActionBasic.h"
#include "Scripts/Instance/IEntity/IEntity.h"

#include "Scripts/Manager/EffectManager.h"

/// <summary>
/// スキルアクション
/// </summary>
class ISkillAction : public IActionBasic {
public:
	enum class TargetType : i8 {
		Target,
		Ground,
		Self,
	};

public:
	ISkillAction() noexcept = default;
	virtual ~ISkillAction() = default;

public:
	void load_from_json(const std::filesystem::path& file);

protected:
	szg::JsonAsset json;
	TargetType targetType{};
	r32 castTime;
	r32 recastTime;
	i32 mpCost{};
	float range{};
	float radius{};

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif // DEBUG_FEATURES_ENABLE

public:
	static void SetEffectManager(Reference<EffectManager> manager);

protected:
	static inline Reference<EffectManager> effectManager{ nullptr };
};
