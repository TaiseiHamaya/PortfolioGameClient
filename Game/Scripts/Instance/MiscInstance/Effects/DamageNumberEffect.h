#pragma once

#include "./IEffectInstance.h"

#include <Library/Math/ColorRGBA.h>

namespace szg {

class StringRectInstance;

}

/// <summary>
/// ダメージ数値エフェクト
/// キャラクターが受けたダメージの数値を画面上に表示するエフェクトを管理
/// </summary>
class DamageNumberEffect final : public IEffectInstance {
public:
	DamageNumberEffect() noexcept = default;
	~DamageNumberEffect() noexcept override = default;

	SZG_CLASS_MOVE_ONLY(DamageNumberEffect)

public:
	void initialize(i32 damage, ColorRGBA color);

	void update() override;

	void update_affine() override;

public:
	void draw_particle() const {};

public:
	void debug_gui() {};

private:
	szg::WorldTimer timer;
	Reference<szg::StringRectInstance> damageNumber;

	r32 OpaqueTime{ 1.0f };
	r32 EffectTime{ 1.5f };

public:
	static inline Reference<const szg::WorldInstance> cameraInstance{};
};
