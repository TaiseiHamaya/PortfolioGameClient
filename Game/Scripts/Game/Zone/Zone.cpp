#include "Zone.h"

#include <Engine/Module/World/WorldInstance/WorldInstance.h>

#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Instance/Player/Player.h"

Reference<EntityManager> Zone::entity_manager_mut() noexcept {
	return entityManager;
}

Reference<const EntityManager> Zone::entity_manager_imm() const noexcept {
	return entityManager;
}

Reference<EnemyManager> Zone::enemy_manager_mut() noexcept {
	return enemyManager;
}

Reference<const EnemyManager> Zone::enemy_manager_imm() const noexcept {
	return enemyManager;
}

Reference<EffectManager> Zone::effect_manager_mut() noexcept {
	return effectManager;
}

Reference<const EffectManager> Zone::effect_manager_imm() const noexcept {
	return effectManager;
}

Reference<Player> Zone::player_mut() noexcept {
	return player;
}

Reference<const Player> Zone::player_imm() const noexcept {
	return player;
}

Reference<const szg::WorldInstance> Zone::camera_instance_imm() const noexcept {
	return cameraInstance;
}

void Zone::set_entity_manager(Reference<EntityManager> v) noexcept {
	entityManager = v;
}

void Zone::set_enemy_manager(Reference<EnemyManager> v) noexcept {
	enemyManager = v;
}

void Zone::set_effect_manager(Reference<EffectManager> v) noexcept {
	effectManager = v;
}

void Zone::set_player(Reference<Player> v) noexcept {
	player = v;
}

void Zone::set_camera_instance(Reference<const szg::WorldInstance> v) noexcept {
	cameraInstance = v;
}
