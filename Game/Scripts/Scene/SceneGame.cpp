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

#include "Scripts/Extension/RenderNode/EnvironmentMeshNode/EnvironmentMeshNode.h"
#include "Scripts/Extension/Util/LookAtRect.h"
#include "Scripts/Instance/IEntity/ISkillAction/ISkillAction.h"

#include <Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h>
#include <Engine/Module/Render/RenderPSO/Forward/FontRenderingNode/FontRenderingNode.h>

SceneGame::SceneGame() noexcept {
	sceneName = "MainGame";
}

void SceneGame::load_asset() {
	PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/skydome.gltf");
	PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Comet.obj");
	PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	NodeAnimationLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	SkeletonLibrary::RegisterLoadQue("./Game/Resources/Game/Models/Player.gltf");
	PolygonMeshLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	NodeAnimationLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	SkeletonLibrary::RegisterLoadQue("./Game/Resources/Game/Models/RedComet.gltf");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/Circle.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/white.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/shadow.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/AOEBase.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/DustCloud1.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/DustCloud2.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/Fire.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround1.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround2.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/CometGround3.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetCenter6.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetShining.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetAbsorption.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/PlayerEffect/PaladinHolySpiritEffectTargetLight.png");
	TextureLibrary::RegisterLoadQue("./Game/Resources/Game/Texture/rogland_clear_night_2k.dds");
	FontAtlasMSDFLibrary::RegisterLoadQue("./Game/Resources/Font/UDEVGothic35HS-Regular.mtsdf");

	PolygonMeshLibrary::RegisterLoadQue(".\\DirectXGame\\EngineResources\\Models\\Grid\\Grid.obj");

	PrimitiveGeometryLibrary::Transfer("Cubemap", std::make_shared<PrimitiveGeometryAsset>("Cubemap.json"));

	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/FullscreenShader.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Mesh/StaticMesh.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Mesh/SkinningMesh.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Deferred.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Lighting/NonLighting.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Lighting/DirectionalLighting.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Deferred/Lighting/PointLighting.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/Primitive/Rect3d.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/ForwardAlpha.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/Particle/ParticleBillboard/ParticleBillboard.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/Particle/ParticleBillboard/ParticleBillboard.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Posteffect/RadialBlur/RadialBlur.PS.hlsl");

	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/Mesh/Skybox/Skybox.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/Mesh/Skybox/Skybox.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/Bloom.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/DownSampling.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/GaussianBlur.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/LuminanceExtraction.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./Game/Resources/HLSL/MargeTexture4.PS.hlsl");

	EnvironmentMeshNode::BeginLoadShader();
	FontRenderingNode::BeginLoadShader();
}

void SceneGame::custom_setup() {
	// その他
	std::unique_ptr<EntityManager> entityManager; // 全てのエンティティ管理
	std::unique_ptr<EffectManager> effectManager; // エフェクト管理
	std::unique_ptr<EnemyManager> enemyManager; // 敵管理

	std::unique_ptr<GameInputHandler> gameInputHandler; // プレイヤー入力
	std::unique_ptr<NetworkCluster> networkCluster; // ネットワーク
	std::unique_ptr<ZoneHandler> zoneHandler; // ゾーン処理
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

	// Reference
	Reference<EntityManager> entityManagerRef = entityManager;
	Reference<EnemyManager> enemyManagerRef = enemyManager;
	Reference<EffectManager> effectManagerRef = effectManager;
	Reference<NetworkCluster> networkClusterRef = networkCluster;
	Reference<GameInputHandler> gameInputHandlerRef = gameInputHandler;
	Reference<ZoneHandler> zoneHandlerRef = zoneHandler;

	sceneScriptManager.register_script(std::move(gameInputHandler));
	sceneScriptManager.register_script(std::move(zoneHandler));
	sceneScriptManager.register_script(std::move(networkCluster));
	sceneScriptManager.register_script(std::move(enemyManager));
	sceneScriptManager.register_script(std::move(entityManager));
	sceneScriptManager.register_script(std::move(effectManager));

	environmentMeshExecutor = eps::CreateUnique<EnvironmentMeshExecutor>("Grid.obj", 1, "rogland_clear_night_2k.dds");

	// Setup
	enemyManagerRef->setup(entityManagerRef);
	zoneHandlerRef->setup(entityManagerRef, enemyManagerRef, networkClusterRef->connection_manager(), networkClusterRef->get_receiver(), networkClusterRef->get_sender());
	networkClusterRef->setup();
	gameInputHandlerRef->setup(zoneHandlerRef);

	if (!get_world(0)) {
		return;
	}

	entityManagerRef->setup(get_world(0)->world_root_mut());
	// WorldInstances
	// Allocation
	directionalLight = get_world(0)->world_root_mut().instantiate<DirectionalLightInstance>();
	directionalLight->set_influence_layer(1);
	directionalLight->light_data_mut().direction = CVector3::DOWN;
	directionalLight->light_data_mut().intensity = 0.5f;
	Reference<Player> player = entityManagerRef->generate<Player>("Player.json");
	skydome = get_world(0)->world_root_mut().instantiate<StaticMeshInstance>(nullptr, "skydome.gltf");
	camera3D = get_world(0)->world_root_mut().instantiate<FollowCamera>();
	testString = get_world(0)->world_root_mut().instantiate<StringRectInstance>();

	LookAtRect::camera = camera3D;
	Particle::lookAtDefault = camera3D.ptr();
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
	directionalLight->light_data_mut().intensity = 0.500f;
	testString->initialize("UDEVGothic35HS-Regular.mtsdf", 10.0f, CVector2::ZERO);
	testString->set_string("MSDF Font Rendering Test ひらがなもできます");
	testString->get_transform().set_translate_y(1);

	cometEffect = std::make_unique<CometEffect>();
}
