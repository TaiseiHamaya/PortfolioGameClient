#include "PaladinHolySpiritEffectTarget.h"

#include <Engine/Module/World/WorldManager.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Tools/Easing.h>
#include <Library/Utility/Tools/Functions.h>
#include <Library/Utility/Tools/MathEPS.h>

PaladinHolySpiritEffectTarget::PaladinHolySpiritEffectTarget() : IEffectInstance() {}

void PaladinHolySpiritEffectTarget::initialize(const Vector3& position) {
	// 位置
	transform.set_translate(position);
	update_affine();

	// 黒いやつ
	absorption = world_manager()->create<LookAtRect>(this);
	absorption->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetAbsorption.png");
	absorption->initialize(Vector2{ 7.0f,7.0f }, Vector2{ 0.5f,0.5f });
	absorption->get_transform().set_scale(CVector3::ZERO);
	absorption->get_material().color = CColor4::BLACK;
	absorption->get_material().lightingType = LighingType::None;

	// でかいやつ
	centerConstraint = world_manager()->create<WorldInstance>(this);
	centerBillboards.resize(6);
	for (i32 i = 0; std::unique_ptr<Rect3d>& centerBillboard : centerBillboards) {
		centerBillboard = world_manager()->create<Rect3d>(centerConstraint);
		centerBillboard->initialize(Vector2{ 3.0f,5.0f }, Vector2{ 0.5f,0.0f });
		centerBillboard->get_transform().set_quaternion(
			Quaternion::AngleAxis(CVector3::FORWARD, PI2 * i / 6)
		);
		centerBillboard->get_transform().set_scale(CVector3::ZERO);
		centerBillboard->get_transform().set_translate({ 0.0,0.0,0.01f });
		centerBillboard->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetCenter6.png");
		centerBillboard->get_material().lightingType = LighingType::None;

		++i;
	}

	// 中心の丸いやつ
	lightBillboard = world_manager()->create<LookAtRect>(this);
	lightBillboard->initialize(Vector2{ 5.5f,5.5f }, Vector2{ 0.5f,0.5f });
	lightBillboard->get_transform().set_scale(CVector3::ZERO);
	lightBillboard->get_transform().set_translate({ 0.0,0.0,0.02f });
	lightBillboard->get_material().color = { 0.334f,0.462f,0.853f,0.0f };
	lightBillboard->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetLight.png");
	lightBillboard->get_material().lightingType = LighingType::None;

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
	if (timer >= 0.15f) {
		r32 param = eps::saturate((timer - 0.15f) / (1.25f - 0.15f));
		r32 scaleBase = eps::lerp(0.0f, 1.0f, Easing::Out::Cubic(param));
		lightBillboard->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		lightBillboard->get_material().color.alpha = std::sin(param * PI);
	}
	// 周りの黒いやつ
	if (timer >= 0.35f) {
		r32 param = eps::saturate((timer - 0.35f) / (0.7f - 0.35f));
		r32 scaleBase = eps::lerp(1.0f, 0.0f, param);
		absorption->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		absorption->get_material().color.alpha = eps::lerp(0.0f, 1.0f, Easing::Out::Expo(param));
	}
	// 一番でかい派手なやつ
	if (timer >= 0.25f) {
		centerConstraint->look_at(LookAtRect::camera);
		r32 param = eps::saturate((timer - 0.25f) / (1.8f - 0.25f));
		for (std::unique_ptr<Rect3d>& centerBillboard : centerBillboards) {
			centerBillboard->get_transform().set_scale({
				param,
				Easing::Out::Back(param),
				1.0f
			});
			centerBillboard->get_material().color =
				Color4::Lerp({ 0.334f,0.462f,0.853f,1.0f }, CColor4::ZERO, Easing::In::Cubic(param));
			centerBillboard->get_material().color.alpha = eps::lerp(1.0f, 0.0f, Easing::In::Expo(param));
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

#ifdef DEBUG_FEATURES_ENABLE

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

#endif //DEBUG_FEATURES_ENABLE
