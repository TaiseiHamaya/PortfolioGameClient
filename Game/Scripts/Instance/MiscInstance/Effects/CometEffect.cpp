#include "CometEffect.h"

#include <Engine/Module/Manager/World/WorldRoot.h>
#include <Engine/Runtime/Clock/WorldClock.h>

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

#include <Engine/Module/Render/RenderPSO/Posteffect/RadialBlur/RadialBlurNode.h>

#include <Library/Math/VectorConverter.h>
#include <Library/Utility/Tools/MathEPS.h>
#include <Library/Utility/Tools/RandomEngine.h>

#define COLOR3_SERIALIZER
#define COLOR4_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

CometEffect::CometEffect() = default;
CometEffect::~CometEffect() = default;

void CometEffect::initialize(const Vector3& position, Reference<BlurInfo> blur_) {
	const auto& worldRoot = world_root_mut();
	transform.set_translate(position);
	transform.set_translate_y(0.02f);

	// 煙のパーティクル1
	dustCloudParticle0 = worldRoot->instantiate<ParticleEmitterInstance>(this, "DustCloud0.json", 100);
	dustCloudParticle0->set_active(false);

	// 煙のパーティクル1
	dustCloudParticle1 = worldRoot->instantiate<ParticleEmitterInstance>(this, "DustCloud1.json", 100);
	dustCloudParticle1->set_active(false);

	json.load("Action/RedComet/CometEffect.json");
	json.register_value(__JSON_RESOURCE_REGISTER(CometHight));
	json.register_value(__JSON_RESOURCE_REGISTER(CometBodyColor));
	json.register_value(__JSON_RESOURCE_REGISTER(CometFireSize));
	json.register_value(__JSON_RESOURCE_REGISTER(CometFirePivot));
	json.register_value(__JSON_RESOURCE_REGISTER(CometFireColor));
	json.register_value(__JSON_RESOURCE_REGISTER(GroundEffectSize));
	json.register_value(__JSON_RESOURCE_REGISTER(GroundEffectPivot));
	json.register_value(__JSON_RESOURCE_REGISTER(GroundEffectColor));
	json.register_value(__JSON_RESOURCE_REGISTER(FallTime));
	json.register_value(__JSON_RESOURCE_REGISTER(GroundEffectTime));
	json.register_value(__JSON_RESOURCE_REGISTER(BlurLengthMax));
	json.register_value(__JSON_RESOURCE_REGISTER(BlurWeight));

	// コメットの隕石部分
	cometBody = worldRoot->instantiate<StaticMeshInstance>(this);
	cometBody->reset_mesh("Comet.obj");
	cometBody->set_active(false);
	cometBody->get_materials()[0].color = CometBodyColor;
	cometBody->get_materials()[0].lightingType = LighingType::None;
	// コメットの炎エフェクト
	cometFire = worldRoot->instantiate<Rect3d>(this);
	cometFire->set_active(false);
	cometFire->initialize(CometFireSize, CometFirePivot);
	cometFire->get_material().color = CometFireColor;
	cometFire->get_material().lightingType = LighingType::None;
	cometFire->get_material().texture = TextureLibrary::GetTexture("Fire.png");
	// 隕石と地面が衝突した時に出すエフェクト
	groundEffect = worldRoot->instantiate<Rect3d>(this);
	groundEffect->set_active(false);
	groundEffect->initialize(GroundEffectSize, GroundEffectPivot);
	groundEffect->get_material().color = GroundEffectColor;
	groundEffect->get_material().lightingType = LighingType::None;
	groundEffect->get_material().texture = TextureLibrary::GetTexture("CometGround3.png");
	groundEffect->get_transform().set_quaternion(
		Quaternion::AngleAxis(CVector3::BASIS_Y, RandomEngine::Random01MOD() * PI2)
		* Quaternion::LookForward(CVector3::UP, CVector3::FORWARD)
	);

	blurData = blur_;
}

void CometEffect::update() {
	timer.ahead();

	if (timer.time() <= FallTime) {
		cometBody->set_active(true);
		cometFire->set_active(true);
		float param = timer.time() / FallTime;
		float posY = std::lerp(CometHight, 0.0f, param);
		cometBody->get_transform().set_translate_y(posY);
		cometFire->get_transform().set_translate_y(posY);
		// カメラ方向に向ける
		Vector3 forward = CVector3::FORWARD * camera->world_affine().get_basis();
		forward.y = 0;
		forward = -forward.normalize();
		cometFire->get_transform().set_quaternion(
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
		groundEffect->get_transform().set_scale(CVector3::BASIS * param);
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

	if (dustCloudParticle0->is_end_all() && dustCloudParticle1->is_end_all()) {
		isDestroy = true;
	}
}

void CometEffect::draw_particle() const {
	dustCloudParticle0->transfer();
	dustCloudParticle1->transfer();
	dustCloudParticle0->draw();
	dustCloudParticle1->draw();
}

bool CometEffect::is_end() const {
	return isDestroy;
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