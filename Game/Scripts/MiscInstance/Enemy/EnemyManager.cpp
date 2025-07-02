#include "EnemyManager.h"

#include <limits>

#include "./Enemy.h"
#include "Scripts/Manager/EntityManager.h"

void EnemyManager::setup(Reference<EntityManager> entityManager_) {
	entityManager = entityManager_;
}

void EnemyManager::generate(u64 id, const std::filesystem::path& path, Vector3 position) {
	Reference<Enemy> enemy =
		enemies.emplace_back(
			entityManager->generate<Enemy>(id, path)
		);
	enemy->get_transform().set_translate(position);
	enemy->get_transform().set_quaternion(
		Quaternion::LookForward(CVector3::BACKWARD)
	);
}

Reference<Enemy> EnemyManager::get_nearest(const Vector3& position) const {
	Reference<Enemy> result{};
	float currentsDistance = std::numeric_limits<float>::max();
	for (Reference<Enemy> enemy : enemies) {
		float distance = (enemy->world_position() - position).length();
		if (distance < currentsDistance) {
			currentsDistance = distance;
			result = enemy;
		}
	}

	return result;
}

#ifdef DEBUG_FEATURES_ENABLE
void EnemyManager::debug_gui() {
	ImGui::Begin("Enemy");
	enemies.front()->debug_gui();
	ImGui::End();
}
#endif // DEBUG_FEATURES_ENABLE
