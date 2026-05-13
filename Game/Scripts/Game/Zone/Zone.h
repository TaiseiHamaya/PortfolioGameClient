#pragma once

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class EntityManager;
class EnemyManager;
class EffectManager;
class Player;

namespace szg {
class WorldInstance;
}

/// <summary>
/// ゾーンのコンテキスト
/// </summary>
class Zone {
public:
	Zone() = default;
	~Zone() = default;

	SZG_CLASS_MOVE_ONLY(Zone)

public:
	Reference<EntityManager> entity_manager_mut() noexcept;
	Reference<const EntityManager> entity_manager_imm() const noexcept;

	Reference<EnemyManager> enemy_manager_mut() noexcept;
	Reference<const EnemyManager> enemy_manager_imm() const noexcept;

	Reference<EffectManager> effect_manager_mut() noexcept;
	Reference<const EffectManager> effect_manager_imm() const noexcept;

	Reference<Player> player_mut() noexcept;
	Reference<const Player> player_imm() const noexcept;

	Reference<const szg::WorldInstance> camera_instance_imm() const noexcept;

public:
	void set_entity_manager(Reference<EntityManager> v) noexcept;
	void set_enemy_manager(Reference<EnemyManager> v) noexcept;
	void set_effect_manager(Reference<EffectManager> v) noexcept;
	void set_player(Reference<Player> v) noexcept;
	void set_camera_instance(Reference<const szg::WorldInstance> v) noexcept;

private:
	Reference<EntityManager> entityManager;
	Reference<EnemyManager> enemyManager;
	Reference<EffectManager> effectManager;
	Reference<Player> player;
	Reference<const szg::WorldInstance> cameraInstance;
};
