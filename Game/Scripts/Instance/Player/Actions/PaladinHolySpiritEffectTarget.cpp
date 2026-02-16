#include "PaladinHolySpiritEffectTarget.h"

#include <Engine/Module/Manager/World/WorldRoot.h>

#include <Library/Math/Definition.h>
#include <Library/Utility/Tools/Easing.h>
#include <Library/Utility/Tools/MathEPS.h>

PaladinHolySpiritEffectTarget::PaladinHolySpiritEffectTarget() : IEffectInstance() {}

void PaladinHolySpiritEffectTarget::initialize(const Vector3& position) {
	// 位置
	transform.set_translate(position);
	update_affine();

	load_constant_values();

	Reference<szg::WorldRoot> worldRoot = world_root_mut();
	// 黒いやつ
	absorption = worldRoot->instantiate<LookAtRect>(this);
	absorption->get_material().texture = szg::TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetAbsorption.png");
	absorption->initialize(absorptionEffectValues.size, absorptionEffectValues.pivot);
	absorption->transform_mut().set_scale(absorptionEffectValues.beginScale);
	absorption->get_material().color = absorptionEffectValues.color;
	absorption->get_material().lightingType = LighingType::None;
	absorption->set_active(false);

	// でかいやつ
	centerConstraint = worldRoot->instantiate<WorldInstance>(this);
	centerBillboards.resize(6);
	for (i32 i = 0; Reference<szg::Rect3d>& centerBillboard : centerBillboards) {
		centerBillboard = worldRoot->instantiate<szg::Rect3d>(centerConstraint);
		centerBillboard->initialize(centerEffectValues.size, centerEffectValues.pivot);
		centerBillboard->transform_mut().set_scale(centerEffectValues.beginScale);
		centerBillboard->transform_mut().set_quaternion(
			Quaternion::AngleAxis(CVector3::FORWARD, PI2 * i / 6)
		);
		centerBillboard->transform_mut().set_translate({ 0.0,0.0,0.01f });
		centerBillboard->get_material().texture = szg::TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetCenter6.png");
		centerBillboard->get_material().lightingType = LighingType::None;
		centerBillboard->set_active(false);

		++i;
	}

	// 中心の丸いやつ
	lightBillboard = worldRoot->instantiate<LookAtRect>(this);
	lightBillboard->initialize(lightEffectValues.size, lightEffectValues.pivot);
	lightBillboard->transform_mut().set_translate({ 0.0,0.0,0.02f });
	lightBillboard->transform_mut().set_scale(lightEffectValues.beginScale);
	lightBillboard->get_material().color = lightEffectValues.color;
	lightBillboard->get_material().texture = szg::TextureLibrary::GetTexture("PaladinHolySpiritEffectTargetLight.png");
	lightBillboard->get_material().lightingType = LighingType::None;
	lightBillboard->set_active(false);

	// エーテルエフェクト
	etherDustEmitter = worldRoot->instantiate<szg::ParticleEmitterInstance>(this, "PaladinHolySpiritEffectTargetEther.json", 128);
	etherDustEmitter->update_affine();
	etherDustEmitter->set_active(false);

	// キラキラしたやつ
	shiningEmitter = worldRoot->instantiate<szg::ParticleEmitterInstance>(this, "PaladinHolySpiritEffectTargetShining.json", 8);
	shiningEmitter->update_affine();
	shiningEmitter->set_active(false);
}

void PaladinHolySpiritEffectTarget::update() {
	timer.ahead();

	// 中心の光ってるやつ
	if (timer >= lightEffectValues.beginTime) {
		r32 param = eps::saturate((timer - lightEffectValues.beginTime) / (lightEffectValues.endTime - lightEffectValues.beginTime));
		r32 scaleBase = eps::lerp(0.0f, 1.0f, Easing::Out::Cubic(param));
		lightBillboard->transform_mut().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		lightBillboard->get_material().color.alpha = std::sin(param * PI);
		if (timer < lightEffectValues.endTime) {
			lightBillboard->set_active(true);
		}
		else {
			lightBillboard->set_active(false);
		}
	}
	// 周りの黒いやつ
	if (timer >= absorptionEffectValues.beginTime) {
		r32 param = eps::saturate((timer - absorptionEffectValues.beginTime) / (absorptionEffectValues.endTime - absorptionEffectValues.beginTime));
		r32 scaleBase = eps::lerp(1.0f, 0.0f, param);
		absorption->transform_mut().set_scale(Vector3{ scaleBase, scaleBase, 1.0f });
		absorption->get_material().color.alpha = eps::lerp(0.0f, 1.0f, Easing::Out::Expo(param));
		if (timer < absorptionEffectValues.endTime) {
			absorption->set_active(true);
		}
		else {
			absorption->set_active(false);
		}
	}
	// 一番でかい派手なやつ
	if (timer >= centerEffectValues.beginTime) {
		centerConstraint->look_at(LookAtRect::camera);
		r32 param = eps::saturate((timer - centerEffectValues.beginTime) / (centerEffectValues.endTime - centerEffectValues.beginTime));
		for (Reference<szg::Rect3d>& centerBillboard : centerBillboards) {
			centerBillboard->transform_mut().set_scale({
				param,
				Easing::Out::Back(param),
				1.0f
			});
			centerBillboard->get_material().color =
				ColorRGBA::Lerp(centerEffectValues.color, CColorRGBA::ZERO, Easing::In::Cubic(param));
			centerBillboard->get_material().color.alpha = eps::lerp(1.0f, 0.0f, Easing::In::Expo(param));
		}
		if (timer < centerEffectValues.endTime) {
			for (auto& centerBillboard : centerBillboards) {
				centerBillboard->set_active(true);
			}
		}
		else {
			for (auto& centerBillboard : centerBillboards) {
				centerBillboard->set_active(false);
			}
		}
	}
	//// 煙のパーティクル
	if (timer.just_crossed(0.6f)) {
		etherDustEmitter->set_active(true);
	}
	// 最後のキラキラしたやつ
	if (timer.just_crossed(1.0f)) {
		shiningEmitter->set_active(true);
	}

	etherDustEmitter->update();
	shiningEmitter->update();

	etherDustEmitter->transfer();
	shiningEmitter->transfer();

	if (etherDustEmitter->is_end_all() && shiningEmitter->is_end_all()) {
		isEnded = true;
	}
}

void PaladinHolySpiritEffectTarget::draw_particle() const {
	etherDustEmitter->draw();
	shiningEmitter->draw();
}

void PaladinHolySpiritEffectTarget::load_constant_values() {
	// エフェクト値設定
	json.load("[[game]]\\Action\\Player\\Paladin\\HolySpiritTarget.json");
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
