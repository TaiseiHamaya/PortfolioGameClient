#include "SceneGame.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include <Engine/Assets/Shader/ShaderLibrary.h>

#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationLibrary.h>
#include <Engine/Assets/Animation/Skeleton/SkeletonLibrary.h>
#include <Engine/Assets/Audio/AudioLibrary.h>
#include <Engine/Assets/PolygonMesh/PolygonMeshLibrary.h>
#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h>

#include "Scripts/Extension/RenderNode/EnvironmentMeshNode/EnvironmentMeshNode.h"
#include "Scripts/Extension/Util/LookAtRect.h"
#include "Scripts/Game/GameInputHandler.h"
#include "Scripts/Game/Zone/ZoneHandler.h"
#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Manager/EntityManager.h"
#include "Scripts/Manager/GameLogWindowManager.h"
#include "Scripts/Network/NetworkCluster.h"

#include <Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h>

SceneGame::SceneGame() noexcept {
	sceneName = "MainGame";
}

void SceneGame::custom_load_asset() {
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/skydome.gltf");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Comet.obj");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	szg::NodeAnimationLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	szg::SkeletonLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	szg::PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	szg::NodeAnimationLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	szg::SkeletonLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/Circle.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/white.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/shadow.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/AOEBase.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/DustCloud1.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/DustCloud2.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/Fire.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround1.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround2.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround3.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetCenter6.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetShining.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetAbsorption.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetLight.png");
	szg::TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/rogland_clear_night_2k.dds");
	szg::FontAtlasMSDFLibrary::RegisterLoadQue("./Game/Resources/Font/UDEVGothic35HS-Regular.mtsdf");

	szg::PolygonMeshLibrary::RegisterLoadQue(".\\DirectXGame\\EngineResources\\Models\\Grid\\Grid.obj");

	szg::PrimitiveGeometryLibrary::Transfer("Cubemap", std::make_shared<szg::PrimitiveGeometryAsset>("Cubemap.json"));

	szg::ShaderLibrary::RegisterLoadQue("./SyzygyEngine/EngineResources/HLSL/Forward/Particle/ParticleBillboard/ParticleBillboard.VS.hlsl");
	szg::ShaderLibrary::RegisterLoadQue("./SyzygyEngine/EngineResources/HLSL/Forward/Particle/ParticleBillboard/ParticleBillboard.PS.hlsl");

	szg::ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/Mesh/Skybox/Skybox.VS.hlsl");
	szg::ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/Mesh/Skybox/Skybox.PS.hlsl");

	EnvironmentMeshNode::BeginLoadShader();
}

void SceneGame::custom_setup() {
	// その他
	std::unique_ptr<EntityManager> entityManager; // 全てのエンティティ管理
	std::unique_ptr<EffectManager> effectManager; // エフェクト管理
	std::unique_ptr<EnemyManager> enemyManager; // 敵管理

	std::unique_ptr<GameInputHandler> gameInputHandler; // プレイヤー入力
	std::unique_ptr<NetworkCluster> networkCluster; // ネットワーク
	std::unique_ptr<ZoneHandler> zoneHandler; // ゾーン処理

	std::unique_ptr<GameLogWindowManager> gameLogWindowManager;
	// ---------- Initialize ----------
	// WorldManager
	entityManager = eps::CreateUnique<EntityManager>();
	enemyManager = eps::CreateUnique<EnemyManager>();
	effectManager = eps::CreateUnique<EffectManager>();

	networkCluster = eps::CreateUnique<NetworkCluster>();
	zoneHandler = eps::CreateUnique<ZoneHandler>();

	// Network
	networkCluster->initialize();

	gameInputHandler = eps::CreateUnique<GameInputHandler>();
	gameInputHandler->initialize();

	gameLogWindowManager = eps::CreateUnique<GameLogWindowManager>();

	// Reference
	Reference<EntityManager> entityManagerRef = entityManager;
	Reference<EnemyManager> enemyManagerRef = enemyManager;
	Reference<EffectManager> effectManagerRef = effectManager;
	Reference<NetworkCluster> networkClusterRef = networkCluster;
	Reference<GameInputHandler> gameInputHandlerRef = gameInputHandler;
	Reference<ZoneHandler> zoneHandlerRef = zoneHandler;
	Reference<GameLogWindowManager> gameLogWindowManagerRef = gameLogWindowManager;

	sceneScriptManager.register_script(std::move(gameInputHandler));
	sceneScriptManager.register_script(std::move(zoneHandler));
	sceneScriptManager.register_script(std::move(networkCluster));
	sceneScriptManager.register_script(std::move(enemyManager));
	sceneScriptManager.register_script(std::move(entityManager));
	sceneScriptManager.register_script(std::move(effectManager));
	sceneScriptManager.register_script(std::move(gameLogWindowManager));

	environmentMeshExecutor = eps::CreateUnique<EnvironmentMeshExecutor>("Grid.obj", 1, "rogland_clear_night_2k.dds");

	gameLogWindowManagerRef->initialize();
	// Setup
	enemyManagerRef->setup(entityManagerRef);
	zoneHandlerRef->setup(entityManagerRef, enemyManagerRef, networkClusterRef->connection_manager(), networkClusterRef->get_receiver(), networkClusterRef->get_sender(), gameLogWindowManagerRef);
	std::string userName;
	auto temp = szg::RuntimeStorage::GetValueImm("Temp", "PlayerName");
	if (temp.is_null()) {
		userName = "Unknown";
	}
	else {
		userName = std::any_cast<std::string>(*temp);
	}
	networkClusterRef->setup(userName);
	gameInputHandlerRef->setup(zoneHandlerRef);

	if (!get_world(0)) {
		return;
	}

	entityManagerRef->setup(get_world(0)->world_root_mut());
	// WorldInstances
	// Allocation
	Reference<Player> player = entityManagerRef->generate<Player>("Player.json");
	player->set_name(userName);
	skydome = get_world(0)->world_root_mut().instantiate<szg::StaticMeshInstance>(nullptr, "skydome.gltf");
	camera3D = get_world(0)->world_root_mut().instantiate<FollowCamera>();

	LookAtRect::camera = camera3D;
	szg::Particle::lookAtDefault = camera3D.ptr();
	CometEffect::camera = camera3D;
	ISkillAction::SetEffectManager(effectManagerRef);
	zoneHandlerRef->set_player(player);
	gameInputHandlerRef->set_instances(player, camera3D);
	networkClusterRef->set_player(player);
	//environmentMeshExecutor->setup(directionalLightingExecutor, camera3D);

	skydome->get_transform().set_scale(CVector3::BASIS * 100);
	skydome->get_materials()[0].lightingType = LighingType::None;
	skydome->set_active(false);
	camera3D->initialize();
	camera3D->set_target(player);

	cometEffect = std::make_unique<CometEffect>();
}
