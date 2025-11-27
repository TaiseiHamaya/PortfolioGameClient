#include "PaladinHolySpiritEffectTarget.h"

#include <Engine/Module/World/WorldManager.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Tools/Easing.h>
#include <Library/Utility/Tools/MathEPS.h>

PaladinHolySpiritEffectTarget::PaladinHolySpiritEffectTarget() : IEffectInstance() {}

void PaladinHolySpiritEffectTarget::initialize(const Vector3& position) {
	// 位置
	transform.set_translate(position);
	update_affine();

	// エフェクト値設定
	json.load("Action/Player/Paladin/HolySpiritTarget.json");
	json.register_value("absorption.size", absorptionEffectValues.size);
	json.register_value("absorption.pivot", absorptionEffectValues.pivot);
	json.register_value("absorption.color", absorptionEffectValues.color);
	json.register_value("absorption.beginTime", absorptionEffectValues.beginTime);
	json.register_value("absorption.endTime", absorptionEffectValues.endTime);
	json.register_value("absorption.beginScale", absorptionEffectValues.beginScale);
	json.register_value("center.size", centerEffectValues.size);
	json.register_value("center.pivot", centerEffectValues.pivot);
	json.register_value("center.color", centerEffectValues.color);
	json.register_value("center.beginTime", centerEffectValues.beginTime);
	json.register_value("center.endTime", centerEffectValues.endTime);
	json.register_value("center.beginScale", centerEffectValues.beginScale);
	json.register_value("light.size", lightEffectValues.size);
	json.register_value("light.pivot", lightEffectValues.pivot);
	json.register_value("light.color", lightEffectValues.color);
	json.register_value("light.beginTime", lightEffectValues.beginTime);
	json.register_value("light.endTime", lightEffectValues.endTime);
	json.register_value("light.beginScale", lightEffectValues.beginScale);

	// 黒いやつ
	absorption = world_manager()->create<LookAtRect>(this);
	absorption->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetAbsorption.png");
	absorption->initialize(absorptionEffectValues.size, absorptionEffectValues.pivot);
	absorption->get_transform().set_scale(absorptionEffectValues.beginScale);
	absorption->get_material().color = absorptionEffectValues.color;
	absorption->get_material().lightingType = LighingType::None;
	absorption->set_active(false);

	// でかいやつ
	centerConstraint = world_manager()->create<WorldInstance>(this);
	centerBillboards.resize(6);
	for (i32 i = 0; std::unique_ptr<Rect3d>& centerBillboard : centerBillboards) {
		centerBillboard = world_manager()->create<Rect3d>(centerConstraint);
		centerBillboard->initialize(centerEffectValues.size, centerEffectValues.pivot);
		centerBillboard->get_transform().set_scale(centerEffectValues.beginScale);
		centerBillboard->get_transform().set_quaternion(
			Quaternion::AngleAxis(CVector3::FORWARD, PI2 * i / 6)
		);
		centerBillboard->get_transform().set_translate({ 0.0,0.0,0.01f });
		centerBillboard->get_material().texture = TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetCenter6.png");
		centerBillboard->get_material().lightingType = LighingType::None;
		centerBillboard->set_active(false);

		++i;
	}

	// 中心の丸いやつ
	lightBillboard = world_manager()->create<LookAtRect>(this);
	lightBillboard->initialize(centerEffectValues.size, centerEffectValues.pivot);
	lightBillboard->get_transform().set_translate({ 0.0,0.0,0.02f });
	lightBillboard->get_transform().set_scale(centerEffectValues.beginScale);
	lightBillboard->get_material().color = centerEffectValues.color;
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

	// WorldManagerから削除
	world_manager()->erase(absorption);
	world_manager()->erase(centerConstraint);
	for(auto& centerBillboard : centerBillboards) {
		world_manager()->erase(centerBillboard);
	}
	world_manager()->erase(lightBillboard);
	world_manager()->erase(etherDustEmitter);
	world_manager()->erase(shiningEmitter);
	world_manager()->erase(this);
}

void PaladinHolySpiritEffectTarget::update() {
	timer.ahead();

	// 中心の光ってるやつ
	if (timer >= lightEffectValues.beginTime) {
		r32 param = eps::saturate((timer - lightEffectValues.beginTime) / (lightEffectValues.endTime - lightEffectValues.beginTime));
		r32 scaleBase = eps::lerp(0.0f, 1.0f, Easing::Out::Cubic(param));
		lightBillboard->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		lightBillboard->get_material().color.alpha = std::sin(param * PI);
	}
	// 周りの黒いやつ
	if (timer >= absorptionEffectValues.beginTime) {
		r32 param = eps::saturate((timer - absorptionEffectValues.beginTime) / (absorptionEffectValues.endTime - absorptionEffectValues.beginTime));
		r32 scaleBase = eps::lerp(1.0f, 0.0f, param);
		absorption->get_transform().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		absorption->get_material().color.alpha = eps::lerp(0.0f, 1.0f, Easing::Out::Expo(param));
	}
	// 一番でかい派手なやつ
	if (timer >= centerEffectValues.beginTime) {
		centerConstraint->look_at(LookAtRect::camera);
		r32 param = eps::saturate((timer - centerEffectValues.beginTime) / (centerEffectValues.endTime - centerEffectValues.beginTime));
		for (std::unique_ptr<Rect3d>& centerBillboard : centerBillboards) {
			centerBillboard->get_transform().set_scale({
				param,
				Easing::Out::Back(param),
				1.0f
			});
			centerBillboard->get_material().color =
				Color4::Lerp(centerEffectValues.color, CColor4::ZERO, Easing::In::Cubic(param));
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
	if (timer.just_crossed(lightEffectValues.beginTime)) {
		lightBillboard->set_active(true);
	}
	if (timer.just_crossed(absorptionEffectValues.beginTime)) {
		absorption->set_active(true);
	}
	if (timer.just_crossed(centerEffectValues.beginTime)) {
		for (auto& centerBillboard : centerBillboards) {
			centerBillboard->set_active(true);
		}
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
	json.show_imgui();

	//if (ImGui::TreeNode("Ether")) {
	//	etherDustEmitter->debug_gui();
	//	ImGui::TreePop();
	//}
	//ImGui::Separator();
	//if (ImGui::TreeNode("Shining")) {
	//	shiningEmitter->debug_gui();
	//	ImGui::TreePop();
	//}
}

#endif //DEBUG_FEATURES_ENABLE
