#include "SceneGame.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationLibrary.h>
#include <Engine/Assets/Animation/Skeleton/SkeletonLibrary.h>
#include <Engine/Assets/PolygonMesh/PolygonMeshLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/Runtime/RuntimeStorage/RuntimeStorage.h>

#include "Scripts/Extension/Util/LookAtRect.h"
#include "Scripts/Game/GameInputHandler.h"
#include "Scripts/Game/Zone/ZoneHandler.h"
#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"
#include "Scripts/Instance/MiscInstance/Effects/DamageNumberEffect.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Manager/HeartbeatManager.h"
#include "Scripts/Game/Zone/Command/HeartbeatCommand.h"

SceneGame::SceneGame() noexcept {
	sceneName = "MainGame";
}

void SceneGame::custom_load_asset() {
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Assets/Game/Models/skydome.gltf");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Assets/Game/Models/Comet.obj");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Assets/Game/Models/Player.gltf");
	szg::NodeAnimationLibrary::RegisterLoadQue("./Game/Assets/Game/Models/Player.gltf");
	szg::SkeletonLibrary::RegisterLoadQue("./Game/Assets/Game/Models/Player.gltf");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Assets/Game/Models/RedComet.gltf");
	szg::NodeAnimationLibrary::RegisterLoadQue("./Game/Assets/Game/Models/RedComet.gltf");
	szg::SkeletonLibrary::RegisterLoadQue("./Game/Assets/Game/Models/RedComet.gltf");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/Circle.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/shadow.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/AOEBase.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/DustCloud1.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/DustCloud2.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/Fire.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/CometGround1.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/CometGround2.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/CometGround3.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetCenter6.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetShining.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetAbsorption.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetLight.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Assets/Game/Texture/rogland_clear_night_2k.dds");
}

void SceneGame::custom_setup() {
	// その他
	std::unique_ptr<EntityManager> entityManager; // 全てのエンティティ管理
	std::unique_ptr<EffectManager> effectManager; // エフェクト管理
	std::unique_ptr<EnemyManager> enemyManager; // 敵管理

	std::unique_ptr<GameInputHandler> gameInputHandler; // プレイヤー入力
	std::unique_ptr<ZoneHandler> zoneHandler; // ゾーン処理

	std::unique_ptr<GameLogWindowManager> gameLogWindowManager;

	std::unique_ptr<HeartbeatManager> heartbeatManager;
	// ---------- Initialize ----------
	// WorldManager
	entityManager = eps::CreateUnique<EntityManager>();
	enemyManager = eps::CreateUnique<EnemyManager>();
	effectManager = eps::CreateUnique<EffectManager>();

	zoneHandler = eps::CreateUnique<ZoneHandler>();

	gameInputHandler = eps::CreateUnique<GameInputHandler>();
	gameInputHandler->initialize();

	gameLogWindowManager = eps::CreateUnique<GameLogWindowManager>();

	heartbeatManager = eps::CreateUnique<HeartbeatManager>();

	// Reference
	Reference<EntityManager> entityManagerRef = entityManager;
	Reference<EnemyManager> enemyManagerRef = enemyManager;
	Reference<EffectManager> effectManagerRef = effectManager;
	Reference<GameInputHandler> gameInputHandlerRef = gameInputHandler;
	Reference<ZoneHandler> zoneHandlerRef = zoneHandler;
	Reference<GameLogWindowManager> gameLogWindowManagerRef = gameLogWindowManager;
	Reference<HeartbeatManager> heartbeatManagerRef = heartbeatManager;

	sceneScriptManager.register_script(std::move(gameInputHandler));
	sceneScriptManager.register_script(std::move(zoneHandler));
	sceneScriptManager.register_script(std::move(heartbeatManager));
	sceneScriptManager.register_script(std::move(enemyManager));
	sceneScriptManager.register_script(std::move(entityManager));
	sceneScriptManager.register_script(std::move(effectManager));
	sceneScriptManager.register_script(std::move(gameLogWindowManager));

	environmentMeshExecutor = eps::CreateUnique<EnvironmentMeshExecutor>("Grid.gltf", 1, "rogland_clear_night_2k.dds");

	gameLogWindowManagerRef->initialize();
	// Setup
	enemyManagerRef->setup(entityManagerRef);
	zoneHandlerRef->setup(entityManagerRef, enemyManagerRef, gameLogWindowManagerRef);
	std::string userName;
	auto temp = szg::RuntimeStorage::GetValue<std::string>("Temp", "PlayerName");
	if (temp.has_value()) {
		userName = temp.value();
	}
	else {
		userName = "Unknown";
	}
	gameInputHandlerRef->setup(zoneHandlerRef, zoneHandlerRef->chat_box_imm());

	if (!get_world(0)) {
		return;
	}

	entityManagerRef->setup(get_world(0)->world_root_mut());
	// WorldInstances
	// Allocation
	Reference<Player> player = entityManagerRef->generate<Player>("Player.json");
	player->set_name(userName);
	skydome = get_world(0)->world_root_mut().instantiate<szg::StaticMeshInstance>(nullptr, "skydome.gltf");
	cameraInstance = get_world(0)->world_root_mut().instantiate<FollowCamera>();

	LookAtRect::camera = cameraInstance;
	szg::Particle::lookAtDefault = cameraInstance.ptr();
	CometEffect::camera = cameraInstance;
	DamageNumberEffect::cameraInstance = cameraInstance;
	zoneHandlerRef->set_effect_manager(effectManagerRef);
	zoneHandlerRef->set_camera_instance(cameraInstance);
	ISkillAction::SetEffectManager(effectManagerRef);
	zoneHandlerRef->set_player(player);
	gameInputHandlerRef->set_instances(player, cameraInstance);
	HeartbeatCommand::InitializeHeartbeat(heartbeatManagerRef->heartbeat_mut());
	//environmentMeshExecutor->setup(directionalLightingExecutor, camera3D);

	skydome->transform_mut().set_scale(CVector3::BASIS * 100);
	skydome->get_materials()[0].lightingType = LighingType::None;
	skydome->set_active(false);
	cameraInstance->initialize();
	cameraInstance->set_target(player);

	cometEffect = std::make_unique<CometEffect>();
}
