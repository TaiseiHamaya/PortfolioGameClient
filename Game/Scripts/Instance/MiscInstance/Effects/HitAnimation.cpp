#include "HitAnimation.h"

#include <Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h>
#include <Engine/Module/Manager/World/WorldRoot.h>

HitAnimation::HitAnimation(Vector3&& position) {
	emitter = worldRoot->instantiate<szg::ParticleEmitterInstance>(nullptr, "./Assets/Game/Json/Particles.json", 128);

	hitBillboard = worldRoot->instantiate<LookAtRect>(nullptr);
	hitBillboard->get_material().lightingType = LighingType::None;
	hitBillboard->get_material().texture = szg::TextureLibrary::GetTexture("Circle.png");

	emitter->transform_mut().set_translate(position);
	emitter->update_affine();
	emitter->emit();

	hitBillboard->transform_mut().set_translate(position);
	hitBillboard->fixed_update();
	hitBillboard->update_affine();
	const Quaternion rotation = hitBillboard->transform_mut().get_quaternion();
	hitBillboard->transform_mut().plus_translate(CVector3::BASIS_Z * rotation);
	hitAnimationTimer.set(0);
}

void HitAnimation::begin() {
	hitAnimationTimer.ahead();
	hitBillboard->begin();
}

void HitAnimation::update() {
	hitBillboard->update();
	emitter->update();

	int currentFrame = static_cast<int>(std::floor(hitAnimationTimer.time() / 0.0400f));
	hitBillboard->get_material().uvTransform.set_translate(
		{ std::min(currentFrame, 5) / 6.0f, 0 }
	);
}

bool HitAnimation::is_end() const {
	return emitter->is_end_all() && hitAnimationTimer.time() >= 0.5f;
}

//void LookAtRect::fixed_update() {
//	look_at(*camera.ptr());
//}
