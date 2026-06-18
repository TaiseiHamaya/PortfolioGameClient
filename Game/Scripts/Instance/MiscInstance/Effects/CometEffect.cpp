#include "CometEffect.h"

#include <Engine/Runtime/Scene/World/WorldRoot.h>
#include <Engine/Module/Render/RenderPipeline/Posteffect/RadialBlur/RadialBlurPipeline.h>
#include <Engine/Runtime/RuntimeStorage/RuntimeStorage.h>

#include <Library/Math/VectorConverter.h>
#include <Library/Utility/Tools/MathEPS.h>
#include <Library/Utility/Tools/RandomEngine.h>

#define COLOR_RGB_SERIALIZER
#define COLOR_RGBA_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

CometEffect::CometEffect() = default;
CometEffect::~CometEffect() = default;

void CometEffect::initialize(const Vector3& position) {
	Reference<szg::WorldRoot> worldRootMut = world_root_mut();
	transform.set_translate(position);
	transform.set_translate_y(0.02f);

	// 煙のパーティクル1
	dustCloudParticle0 = worldRootMut->instantiate<szg::ParticleEmitterInstance>(this, "DustCloud0.json", 100);
	dustCloudParticle0->set_active(false);

	// 煙のパーティクル1
	dustCloudParticle1 = worldRootMut->instantiate<szg::ParticleEmitterInstance>(this, "DustCloud1.json", 100);
	dustCloudParticle1->set_active(false);

	json.load("Action/RedComet/CometEffect.json");
	json.register_value(SZG_JSON_ASSET_REGISTER(CometHeight));
	json.register_value(SZG_JSON_ASSET_REGISTER(CometBodyColor));
	json.register_value(SZG_JSON_ASSET_REGISTER(CometFireSize));
	json.register_value(SZG_JSON_ASSET_REGISTER(CometFirePivot));
	json.register_value(SZG_JSON_ASSET_REGISTER(CometFireColor));
	json.register_value(SZG_JSON_ASSET_REGISTER(GroundEffectSize));
	json.register_value(SZG_JSON_ASSET_REGISTER(GroundEffectPivot));
	json.register_value(SZG_JSON_ASSET_REGISTER(GroundEffectColor));
	json.register_value(SZG_JSON_ASSET_REGISTER(FallTime));
	json.register_value(SZG_JSON_ASSET_REGISTER(GroundEffectTime));
	json.register_value(SZG_JSON_ASSET_REGISTER(BlurLengthMax));
	json.register_value(SZG_JSON_ASSET_REGISTER(BlurWeight));

	// コメットの隕石部分
	cometBody = worldRootMut->instantiate<szg::StaticMeshInstance>(this);
	cometBody->reset_mesh("Comet.obj");
	cometBody->set_active(false);
	cometBody->get_materials()[0].color = CometBodyColor;
	cometBody->get_materials()[0].lightingType = LighingType::None;
	// コメットの炎エフェクト
	cometFire = worldRootMut->instantiate<szg::Rect3d>(this);
	cometFire->set_active(false);
	cometFire->initialize(CometFireSize, CometFirePivot);
	cometFire->get_material().color = CometFireColor;
	cometFire->get_material().lightingType = LighingType::None;
	cometFire->get_material().texture = szg::TextureLibrary::GetTexture("Fire.png");
	// 隕石と地面が衝突した時に出すエフェクト
	groundEffect = worldRootMut->instantiate<szg::Rect3d>(this);
	groundEffect->set_active(false);
	groundEffect->initialize(GroundEffectSize, GroundEffectPivot);
	groundEffect->get_material().color = GroundEffectColor;
	groundEffect->get_material().lightingType = LighingType::None;
	groundEffect->get_material().texture = szg::TextureLibrary::GetTexture("CometGround3.png");
	groundEffect->transform_mut().set_quaternion(
		Quaternion::AngleAxis(CVector3::BASIS_Y, RandomEngine::Random01MOD() * PI2)
		* Quaternion::LookForward(CVector3::UP, CVector3::FORWARD)
	);

	blurData = szg::RuntimeStorage::GetValue<Reference<szg::RadialBlurPipeline::Data>>("PostEffect", "Blur").value();
}

void CometEffect::update() {
	timer.ahead();

	if (timer.time() <= FallTime) {
		cometBody->set_active(true);
		cometFire->set_active(true);
		float param = timer.time() / FallTime;
		float posY = std::lerp(CometHeight, 0.0f, param);
		cometBody->transform_mut().set_translate_y(posY);
		cometFire->transform_mut().set_translate_y(posY);
		// カメラ方向に向ける
		Vector3 forward = CVector3::FORWARD * camera->world_affine().get_basis();
		forward.y = 0;
		forward = -forward.normalize();
		cometFire->transform_mut().set_quaternion(
			Quaternion::LookForward(forward)
		);
	}
	else if (timer.just_crossed(FallTime)) {
		cometBody->set_active(false);
		cometFire->set_active(false);

		dustCloudParticle0->set_active(true);
		dustCloudParticle1->set_active(true);
		groundEffect->set_active(true);
	}
	else {
		float param = eps::lerp_inv(0.0f, GroundEffectTime, timer - FallTime);
		dustCloudParticle0->update();
		dustCloudParticle1->update();
		// 地面のエフェクトをスケールで出現させる
		groundEffect->transform_mut().set_scale(CVector3::BASIS * param);
		// 透明度を下げてフェードアウト
		groundEffect->get_material().color.alpha = 1 - param;
		// ブラーをかける
		Vector2 blurPosition = Converter::ToVector2(
				Transform3D::Homogeneous(
					world_position() + CVector3::BASIS_Y,
					camera->vp_matrix()
				)
		);
		blurPosition += CVector2::BASIS;
		blurPosition /= 2.0f;
		blurData->center = blurPosition;
		blurData->sampleCount = 8;
		blurData->weight = BlurWeight;
		blurData->length = std::sin(param * PI) * BlurLengthMax;
	}

	dustCloudParticle0->transfer();
	dustCloudParticle1->transfer();

	if (dustCloudParticle0->is_end_all() && dustCloudParticle1->is_end_all()) {
		isEnded = true;
	}
}

void CometEffect::draw_particle() const {
	dustCloudParticle0->draw();
	dustCloudParticle1->draw();
}

bool CometEffect::is_end() const {
	return isEnded;
}

#ifdef DEBUG_FEATURES_ENABLE
void CometEffect::debug_gui() {
	json.show_imgui();
	//if (ImGui::TreeNode("0")) {
	//	dustCloudParticle0->debug_gui();
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("1")) {
	//	dustCloudParticle1->debug_gui();
	//	ImGui::TreePop();
	//}
}
#endif // DEBUG_FUATURE_ENABLE