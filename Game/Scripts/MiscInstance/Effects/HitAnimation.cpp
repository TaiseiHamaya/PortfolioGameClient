#include "HitAnimation.h"

#include <Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h>
#include <Engine/Module/World/WorldManager.h>

HitAnimation::HitAnimation(Vector3&& position) {
	emitter = worldManager->create<ParticleEmitterInstance>(nullptr, "./Resources/Game/Json/Particles.json", 128);

	hitBillboard = worldManager->create<LookAtRect>(nullptr);
	hitBillboard->get_material().lightingType = LighingType::None;
	hitBillboard->get_material().texture = TextureLibrary::GetTexture("Circle.png");

	emitter->get_transform().set_translate(position);
	emitter->update_affine();
	emitter->emit();

	hitBillboard->get_transform().set_translate(position);
	hitBillboard->fixed_update();
	hitBillboard->update_affine();
	const Quaternion rotation = hitBillboard->get_transform().get_quaternion();
	hitBillboard->get_transform().plus_translate(CVector3::BASIS_Z * rotation);
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
