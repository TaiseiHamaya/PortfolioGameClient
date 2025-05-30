#include "PaladinHolySpiritEffectTarget.h"

#include <Engine/Module/World/WorldManager.h>

#include <Library/Utility/Tools/MathEPS.h>
#include <Library/Utility/Tools/Functions.h>
#include <Library/Utility/Tools/Easing.h>
#include <Library/Math/Definition.h>

PaladinHolySpiritEffectTarget::PaladinHolySpiritEffectTarget() : IEffectInstance() {}

void PaladinHolySpiritEffectTarget::initialize(const Vector3& position) {
	// 位置
	transform.set_translate(position);
	update_affine();

	// 黒いやつ
	absorption = world_manager()->create<LookAtRect>(this);
	absorption->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
	absorption->get_transform().set_scale(CVector3::ZERO);

	// でかいやつ
	centerConstraint = world_manager()->create<WorldInstance>(this);
	centerBillboards.resize(6);
	for (i32 i = 0; std::unique_ptr<LookAtRect>& centerBillboard : centerBillboards) {
		centerBillboard = world_manager()->create<LookAtRect>(centerConstraint);
		centerBillboard->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
		centerBillboard->get_transform().set_quaternion(
			Quaternion::AngleAxis(CVector3::FORWARD, PI2 * i / 6)
		);
		centerBillboard->get_transform().set_scale(CVector3::ZERO);
		centerBillboard->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetCenter6.png");

		++i;
	}

	// 中心の丸いやつ
	lightBillboard = world_manager()->create<LookAtRect>(this);
	lightBillboard->initialize(Vector2{ 1.0f,1.0f }, Vector2{ 0.5f,0.5f });
	lightBillboard->get_transform().set_scale(CVector3::ZERO);

	// エーテルエフェクト
	etherDustEmitter = world_manager()->create<ParticleEmitterInstance>(this, "PaladinHolySpiritEffectTargetEther.json", 128);
	etherDustEmitter->update_affine();
	etherDustEmitter->set_active(false);

	// キラキラしたやつ
	shiningEmitter = world_manager()->create<ParticleEmitterInstance>(this, "PaladinHolySpiritEffectTargetShining.json", 8);
	shiningEmitter->update_affine();
	shiningEmitter->set_active(false);
}

void PaladinHolySpiritEffectTarget::setup([[maybe_unused]] Reference<StaticMeshDrawManager> meshDrawManager, Reference<Rect3dDrawManager> rectDrawManager) {
	rectDrawManager->register_instance(absorption);
	for (auto& centerBillboard : centerBillboards) {
		rectDrawManager->register_instance(centerBillboard);
	}
	rectDrawManager->register_instance(lightBillboard);
}

void PaladinHolySpiritEffectTarget::terminate([[maybe_unused]] Reference<StaticMeshDrawManager> meshDrawManager, Reference<Rect3dDrawManager> rectDrawManager) {
	rectDrawManager->unregister_instance(absorption);
	for (auto& centerBillboard : centerBillboards) {
		rectDrawManager->unregister_instance(centerBillboard);
	}
	rectDrawManager->unregister_instance(lightBillboard);
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

	etherDustEmitter->update();
	shiningEmitter->update();

	if (etherDustEmitter->is_end_all() && shiningEmitter->is_end_all()) {
		isDestroy = true;
	}
}

void PaladinHolySpiritEffectTarget::draw_particle() const {
	etherDustEmitter->transfer();
	shiningEmitter->transfer();
	etherDustEmitter->draw();
	shiningEmitter->draw();
}

void PaladinHolySpiritEffectTarget::debug_gui() {
	if (ImGui::TreeNode("Ether")) {
		etherDustEmitter->debug_gui();
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("Shining")) {
		shiningEmitter->debug_gui();
		ImGui::TreePop();
	}
}
