#pragma once

#include <filesystem>
#include <list>

#include <Library/Math/Vector3.h>
#include <Library/Utility/Template/Reference.h>

class Enemy;
class EntityManager;

/// <summary>
/// 敵管理クラス
/// 削除するかも
/// </summary>
class EnemyManager {
public:
	EnemyManager() = default;
	~EnemyManager() = default;

public:
	void setup(Reference<EntityManager> entityManager_);

public:
	void generate(const std::filesystem::path& path, Vector3 position);

	Reference<Enemy> get_nearest(const Vector3& position) const;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	std::list<Reference<Enemy>> enemies;

	Reference<EntityManager> entityManager;
};
