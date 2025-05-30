#include "PaladinHolySpiritEffectTarget.h"

#include <Engine/Module/World/WorldManager.h>

#include <Library/Utility/Tools/MathEPS.h>
#include <Library/Utility/Tools/Functions.h>
#include <Library/Utility/Tools/Easing.h>
#include <Library/Math/Definition.h>

void PaladinHolySpiritEffectTarget::initialize() {
	absorption = world_manager()->create<LookAtRect>(this);
	absorption->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
	absorption->get_transform().set_scale(CVector3::ZERO);

	centerConstraint = world_manager()->create<WorldInstance>(this);
	centerBillboards.resize(6);
	for (i32 i = 0; std::unique_ptr<LookAtRect>& centerBillboard : centerBillboards) {
		centerBillboard = world_manager()->create<LookAtRect>(centerConstraint);
		centerBillboard->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
		centerBillboard->get_transform().set_quaternion(
			Quaternion::AngleAxis(CVector3::FORWARD, PI2 * i / 6)
		);
		centerBillboard->get_transform().set_scale(CVector3::ZERO);

		++i;
	}

	lightBillboard = world_manager()->create<LookAtRect>(this);
	lightBillboard->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
	lightBillboard->get_transform().set_scale(CVector3::ZERO);

	etherDustEmitter = world_manager()->create<ParticleEmitterInstance>(this,
		"", 128);
	etherDustEmitter->set_active(false);

	shiningEmitter = world_manager()->create<ParticleEmitterInstance>(this,
		"", 8);
	shiningEmitter->set_active(false);
}

void PaladinHolySpiritEffectTarget::setup(Reference<IEntity> self, Reference<IEntity> target) {
	// エフェクトの位置を設定
	Vector3 worldPosition{};
	Vector3 offset = Vector3{ 0.0f, 0.5f, 0.1f };
	if (target) {
		worldPosition = target->world_position();
		offset *= target->target_radius();
	}
	Vector3 translate{};
	if (self) {
		Vector3 selfBackward = CVector3::BACKWARD * self->world_affine();
		selfBackward.y = 0.0f;
		selfBackward = selfBackward.normalize_safe();
		translate = worldPosition + offset * Quaternion::LookForward(selfBackward);
	}

	transform.set_translate(translate);
}

void PaladinHolySpiritEffectTarget::update() {
	timer.ahead();

	// 中心の光ってるやつ
	if (timer >= 0.15f && timer < 1.2f) {
		r32 param = (timer - 0.15f) / (1.25f - 0.15f);
		r32 scaleBase = eps::lerp(0.0f, 1.0f, Easing::Out::Cubic(param));
		lightBillboard->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		lightBillboard->get_material().color.alpha = std::sin(param * PI);
	}
	// 周りの黒いやつ
	if (timer >= 0.35f && timer < 1.0f) {
		r32 param = (timer - 0.35f) / (1.0f - 0.35f);
		r32 scaleBase = eps::lerp(1.0f, 0.0f, param);
		absorption->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		absorption->get_material().color.alpha = eps::lerp(1.0f, 0.0f, param);
	}
	// 一番でかい派手なやつ
	if (timer >= 0.25f && timer < 1.8f) {
		centerConstraint->look_at(LookAtRect::camera);
		centerConstraint->update_affine();
		r32 param = (timer - 0.25f) / (1.8f - 0.25f);
		for (std::unique_ptr<LookAtRect>& centerBillboard : centerBillboards) {
			centerBillboard->get_transform().set_scale({
				param,
				Easing::Out::Back(param),
				1.0f
			});
			centerBillboard->get_material().color =
				Color4::Lerp(CColor4::WHITE, CColor4::ZERO, param);
		}
	}
	// 煙のパーティクル
	if (timer.just_crossed(0.6f)) {
		etherDustEmitter->set_active(true);
	}
	// 最後のキラキラしたやつ
	if (timer.just_crossed(1.0f)) {
		shiningEmitter->set_active(true);
	}
}
