#include "CometEffect.h"

#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Clock/WorldClock.h>

#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

#include <Engine/Module/Render/RenderNode/Posteffect/RadialBlur/RadialBlurNode.h>

#include <Library/Math/VectorConverter.h>
#include <Library/Utility/Tools/MathEPS.h>
#include <Library/Utility/Tools/RandomEngine.h>

CometEffect::CometEffect() = default;
CometEffect::~CometEffect() = default;

void CometEffect::initialize(const Vector3& position, Reference<BlurInfo> blur_) {
	const auto& worldManager = world_manager();
	transform.set_translate(position);
	transform.set_translate_y(0.02f);

	dustCloudParticle0 = worldManager->create<ParticleEmitterInstance>(this, "DustCloud0.json", 100);
	dustCloudParticle0->set_active(false);

	dustCloudParticle1 = worldManager->create<ParticleEmitterInstance>(this, "DustCloud1.json", 100);
	dustCloudParticle1->set_active(false);

	cometBody = worldManager->create<StaticMeshInstance>(this);
	cometBody->reset_mesh("Comet.obj");
	cometBody->set_active(false);
	cometBody->get_materials()[0].color = Color3{ 0.5f,0.01f,0.01f };
	cometBody->get_materials()[0].lightingType = LighingType::None;

	cometFire = worldManager->create<Rect3d>(this);
	cometFire->set_active(false);
	cometFire->initialize(Vector2{ 3, 7 }, Vector2{ 0.5f, 0.2f });
	cometFire->get_material().color = Color4{ 0.5f,0.1f,0.1f,1.0f };
	cometFire->get_material().lightingType = LighingType::None;
	cometFire->get_material().texture = TextureLibrary::GetTexture("Fire.png");

	groundEffect = worldManager->create<Rect3d>(this);
	groundEffect->set_active(false);
	groundEffect->initialize(Vector2{ 10,10 }, Vector2{ 0.5f, 0.5f });
	groundEffect->get_material().color = Color4{ 0.8f, 0.8f, 0.8f, 0.8f };
	groundEffect->get_material().lightingType = LighingType::None;
	groundEffect->get_material().texture = TextureLibrary::GetTexture("CometGround3.png");
	groundEffect->get_transform().set_quaternion(
		Quaternion::AngleAxis(CVector3::BASIS_Y, RandomEngine::Random01MOD() * PI2)
		* Quaternion::LookForward(CVector3::UP, CVector3::FORWARD)
	);

	blurData = blur_;
}

void CometEffect::setup(Reference<StaticMeshDrawManager> meshDraw, Reference<Rect3dDrawManager> rectDraw) {
	meshDraw->register_instance(cometBody);
	rectDraw->register_instance(cometFire);
	rectDraw->register_instance(groundEffect);
}

void CometEffect::terminate(Reference<StaticMeshDrawManager> meshDraw, Reference<Rect3dDrawManager> rectDraw) {
	meshDraw->unregister_instance(cometBody);
	rectDraw->unregister_instance(cometFire);
	rectDraw->unregister_instance(groundEffect);

	Reference<WorldManager> worldManager = world_manager();
	worldManager->erase(dustCloudParticle0);
	worldManager->erase(dustCloudParticle1);
	worldManager->erase(cometBody);
	worldManager->erase(cometFire);
	worldManager->erase(groundEffect);
}

void CometEffect::update() {
	timer.ahead();

	constexpr float FallTime = 0.5f;
	if (timer.time() <= FallTime) {
		cometBody->set_active(true);
		cometFire->set_active(true);
		float param = timer.time() / FallTime;
		float posY = std::lerp(20.0f, 0.0f, param);
		cometBody->get_transform().set_translate_y(posY);
		cometFire->get_transform().set_translate_y(posY);
		Vector3 forward = CVector3::FORWARD * camera->world_affine().get_basis();
		forward.y = 0;
		forward = -forward.normalize();
		cometFire->get_transform().set_quaternion(
			Quaternion::LookForward(forward)
		);
	}
	else if (timer.time() - FallTime <= WorldClock::DeltaSeconds()) {
		cometBody->set_active(false);
		cometFire->set_active(false);

		dustCloudParticle0->set_active(true);
		dustCloudParticle1->set_active(true);
		groundEffect->set_active(true);
	}
	else {
		float param = std::clamp((timer.time() - FallTime) * 2, 0.0f, 1.0f);
		dustCloudParticle0->update();
		dustCloudParticle1->update();
		groundEffect->get_transform().set_scale(CVector3::BASIS * param);
		groundEffect->get_material().color.alpha = 1 - param;
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
		blurData->weight = 0.4f;
		blurData->length = std::sin(param * PI) * 0.1f;
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