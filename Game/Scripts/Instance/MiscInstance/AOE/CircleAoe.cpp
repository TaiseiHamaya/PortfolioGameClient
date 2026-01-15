#include "CircleAoe.h"

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/Manager/World/WorldRoot.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Tools/MathEPS.h>

CircleAoe::CircleAoe() = default;
CircleAoe::~CircleAoe() = default;

void CircleAoe::initialize(const Vector3& position, float radius, float duration_) {
	transform.set_translate(position);
	transform.set_translate_y(OFFSET_Y);

	duration = duration_;

	Quaternion rotate = Quaternion::AngleAxis(CVector3::BASIS_X, -PI / 2); // 上を向けtる

	// 範囲表示の基礎部分
	base = world_root_mut()->instantiate<szg::Rect3d>(this);
	base->initialize(CVector3::BASIS * radius, CVector3::HALF);
	base->get_material().texture = szg::TextureLibrary::GetTexture("AOEBase.png");
	base->get_material().lightingType = LighingType::None;
	base->get_material().color.alpha = 0;
	base->transform_mut().set_scale(CVector3::ZERO);
	base->transform_mut().set_quaternion(rotate);
	//base->set_layer(1);
	// なんか動くところ
	effect = world_root_mut()->instantiate<szg::Rect3d>(this);
	effect->initialize(CVector3::BASIS * radius, CVector3::HALF);
	effect->get_material().texture = szg::TextureLibrary::GetTexture("AOEBase.png");
	effect->get_material().lightingType = LighingType::None;
	effect->get_material().color.alpha = 0;
	effect->transform_mut().set_scale(CVector3::ZERO);
	effect->transform_mut().set_quaternion(rotate);
	//effect->set_layer(1);
}

void CircleAoe::update() {
	timer.ahead();

	if (timer < EFFECT_START_TIME) {
		// 表示開始時の拡大
		float param = eps::lerp_inv<r32>(0.0f, EFFECT_START_TIME, timer);
		base->transform_mut().set_scale(Vector3::Lerp(CVector3::ZERO, CVector3::BASIS, param));
		base->get_material().color.alpha = param * AOE_ALPHA;
	}
	else {
		float time = timer - EFFECT_START_TIME;
		float param = time - std::trunc(time); // 小数部を抽出
		base->transform_mut().set_scale(CVector3::BASIS);
		// エフェクトの部分をいい感じに動かす
		effect->transform_mut().set_scale(CVector3::BASIS * param);
		effect->get_material().color.alpha = (1 - param) * AOE_ALPHA;
	}

	if (timer >= duration) {
		// 消える処理
		float param = eps::lerp_inv<r32>(duration, duration + FADE_TIME, timer);
		base->get_material().color.alpha = param * AOE_ALPHA;
		effect->get_material().color.alpha = param * AOE_ALPHA;
	}
}

bool CircleAoe::is_end() const {
	return timer >= duration;
}
