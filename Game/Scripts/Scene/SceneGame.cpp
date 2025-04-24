#include "SceneGame.h"

#include <Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h>
#include <Engine/Module/Render/RenderNode/2D/Sprite/SpriteNode.h>
#include <Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/DeferredAdaptor.h>
#include <Engine/Module/Render/RenderNode/Deferred/Lighting/DirectionalLighingNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/Lighting/NonLightingPixelNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/Mesh/SkinningMeshNodeDeferred.h>
#include <Engine/Module/Render/RenderNode/Deferred/Mesh/StaticMeshNodeDeferred.h>
#include <Engine/Module/Render/RenderNode/Forward/Particle/ParticleBillboardNode/ParticleBillboardNode.h>
#include <Engine/Module/Render/RenderNode/Forward/Primitive/Rect3dNode .h>

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationLibrary.h>
#include <Engine/Assets/Animation/Skeleton/SkeletonLibrary.h>
#include <Engine/Assets/Audio/AudioLibrary.h>
#include <Engine/Assets/PolygonMesh/PolygonMeshLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/Runtime/Scene/SceneManager.h>

#include <Engine/Runtime/Clock/WorldClock.h>

#include "Scripts/Util/LookAtRect.h"

#include <Engine/Debug/DebugValues/DebugValues.h>

void SceneGame::load() {
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

	PolygonMeshLibrary::RegisterLoadQue(".\\DirectXGame\\EngineResources\\Models\\Grid\\Grid.obj");
}

void SceneGame::initialize() {
	// WorldManager
	worldManager = eps::CreateUnique<WorldManager>();
	entityManager = eps::CreateUnique<EntityManager>();
	enemyManager = eps::CreateUnique<EnemyManager>();

	// DrawManager
	skinningMeshDrawManager = eps::CreateUnique<SkinningMeshDrawManager>();
	skinningMeshDrawManager->initialize(1);

	staticMeshDrawManager = eps::CreateUnique<StaticMeshDrawManager>();
	staticMeshDrawManager->initialize(1);

	rect3dDrawManager = eps::CreateUnique<Rect3dDrawManager>();
	rect3dDrawManager->initialize(1);
	rect3dDrawManager->make_instancing(0, 100);

	directionalLightingExecutor = eps::CreateUnique<DirectionalLightingExecutor>(1);

	entityManager->start(worldManager, skinningMeshDrawManager, rect3dDrawManager);
	enemyManager->start(entityManager);

	// WorldInstances
	// Allocation
	directionalLight = worldManager->create<DirectionalLightInstance>();
	player = entityManager->generate<Player>(0, "Player.json");
	skydome = worldManager->create<StaticMeshInstance>(nullptr, "skydome.gltf");
	camera3D = worldManager->create<FollowCamera>();

	LookAtRect::camera = camera3D;
	Particle::lookAtDefault = camera3D.get();
	CometEffect::camera = camera3D;

	skydome->get_transform().set_scale(CVector3::BASIS * 100);
	skydome->get_materials()[0].lightingType = LighingType::None;
	camera3D->initialize();
	camera3D->set_offset({ 0,1,-40 });
	camera3D->set_target(player);
	enemyManager->generate(1, "RedComet.json", Vector3{ 0,0,8 });

	localPlayerCommandHandler = std::make_unique<LocalPlayerCommandHandler>();
	localPlayerCommandHandler->initialize(player);
	localPlayerCommandHandler->start(camera3D);

	// RenderPath
	auto deferredRenderTarget = DeferredAdaptor::CreateGBufferRenderTarget();

	auto deferredMeshNode = eps::CreateShared<StaticMeshNodeDeferred>();
	deferredMeshNode->initialize();
	deferredMeshNode->set_render_target(deferredRenderTarget);
	deferredMeshNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueUseDpehtBefore);

	auto skinMeshNodeDeferred = eps::CreateShared<SkinningMeshNodeDeferred>();
	skinMeshNodeDeferred->initialize();
	skinMeshNodeDeferred->set_render_target(deferredRenderTarget);
	skinMeshNodeDeferred->set_config(RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtAfter);

	auto nonLightingPixelNode = eps::CreateShared<NonLightingPixelNode>();
	nonLightingPixelNode->initialize();
	nonLightingPixelNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	nonLightingPixelNode->set_gbuffers(deferredRenderTarget);

	auto directionalLightingNode = eps::CreateShared<DirectionalLightingNode>();
	directionalLightingNode->initialize();
	directionalLightingNode->set_config(RenderNodeConfig::ContinueDrawAfter);
	directionalLightingNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	directionalLightingNode->set_gbuffers(deferredRenderTarget);

	std::shared_ptr<ParticleBillboardNode> particleBillboardNode;
	particleBillboardNode = std::make_unique<ParticleBillboardNode>();
	particleBillboardNode->initialize();
	particleBillboardNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtBefore | RenderNodeConfig::NoClearDepth);
	particleBillboardNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

	auto rect3dNode = eps::CreateShared<Rect3dNode>();
	rect3dNode->initialize();
	rect3dNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	rect3dNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtAfter | RenderNodeConfig::NoClearDepth);

#ifdef DEBUG_FEATURES_ENABLE
	std::shared_ptr<PrimitiveLineNode> primitiveLineNode;
	primitiveLineNode = std::make_unique<PrimitiveLineNode>();
	primitiveLineNode->initialize();
#endif // _DEBUG

	renderPath = eps::CreateUnique<RenderPath>();
#ifdef DEBUG_FEATURES_ENABLE
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred,nonLightingPixelNode,directionalLightingNode,particleBillboardNode,rect3dNode,primitiveLineNode });
#else
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred,nonLightingPixelNode,directionalLightingNode,particleBillboardNode,rect3dNode });
#endif // DEFERRED_RENDERING

	// CreateInstancing
	skinningMeshDrawManager->make_instancing(0, "Player.gltf", 1);
	skinningMeshDrawManager->make_instancing(0, "Enemy.gltf", 100);
	skinningMeshDrawManager->make_instancing(0, "RedComet.gltf", 1);
	staticMeshDrawManager->make_instancing(0, "skydome.gltf", 1);
	staticMeshDrawManager->make_instancing(0, "Comet.obj", 100);
	staticMeshDrawManager->make_instancing(0, "Grid.obj", 1);

	// RegisterDraw
#ifdef DEBUG_FEATURES_ENABLE
	staticMeshDrawManager->register_debug_instance(0, camera3D, true);
#else
	staticMeshDrawManager->register_instance(DebugValues::GetGridInstance());
#endif // DEBUG_FEATURES_ENABLE
	staticMeshDrawManager->register_instance(skydome);
}

void SceneGame::begin() {
	timer.ahead();

	localPlayerCommandHandler->begin();
	entityManager->begin();
	camera3D->input();
}

void SceneGame::update() {
	localPlayerCommandHandler->update();

	localPlayerCommandHandler->run();

	entityManager->update();
	camera3D->update();

	if (std::fmodf(timer.time(), 2.0f) <= WorldClock::DeltaSeconds()) {
		auto& temp = comets.emplace_back(
			worldManager->create<CircleAoe>(),
			worldManager->create<CometEffect>()
		);
		temp.circleAoE->initialize(player->world_position(), 7, 3);
		temp.circleAoE->start(rect3dDrawManager);
		temp.cometEffect->initialize(player->world_position());
		temp.cometEffect->start(staticMeshDrawManager, rect3dDrawManager);
	}

	for (auto& comet : comets) {
		comet.circleAoE->update();
		if (comet.circleAoE->is_end()) {
			comet.cometEffect->update();
		}
	}
}

void SceneGame::late_update() {
	entityManager->late_update();
}

void SceneGame::begin_rendering() {

	worldManager->update_matrix();

	directionalLight->transfer();
	camera3D->transfer();

	directionalLightingExecutor->begin();
	directionalLightingExecutor->write_to_buffer(directionalLight);
	rect3dDrawManager->transfer();
	staticMeshDrawManager->transfer();
	skinningMeshDrawManager->transfer();
}

void SceneGame::draw() const {
	// 3DMesh
	renderPath->begin();
	camera3D->register_world_projection(2);
	staticMeshDrawManager->draw_layer(0);

	// SkinMesh
	renderPath->next();
	camera3D->register_world_projection(2);
	skinningMeshDrawManager->draw_layer(0);

	// NonLightingPixel
	renderPath->next();

	// DirectionalLight
	renderPath->next();
	camera3D->register_world_lighting(1);
	directionalLightingExecutor->draw_command();

	// ParticleBillboard
	renderPath->next();
	camera3D->register_world_projection(1);
	for (const auto& comet : comets) {
		comet.cometEffect->draw_particle();
	}

	// Rect3D
	renderPath->next();
	camera3D->register_world_projection(3);
	camera3D->register_world_lighting(4);
	directionalLightingExecutor->set_command(5);
	rect3dDrawManager->draw_layer(0);

	renderPath->next();

#ifdef DEBUG_FEATURES_ENABLE
	// LineDraw
	camera3D->register_world_projection(1);
	camera3D->debug_draw_frustum();

	renderPath->next();
#endif // DEFERRED_RENDERING
}

#ifdef DEBUG_FEATURES_ENABLE
void SceneGame::debug_update() {
	ImGui::Begin("Camera3D");
	camera3D->debug_gui();
	ImGui::End();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	directionalLight->debug_gui();
	ImGui::End();

	ImGui::Begin("Player");
	player->debug_gui();
	ImGui::End();

	enemyManager->debug_gui();

	ImGui::Begin("Test");
	if (ImGui::Button("Gen")) {
		auto& temp = comets.emplace_back(
			worldManager->create<CircleAoe>(),
			worldManager->create<CometEffect>()
		);
		temp.circleAoE->initialize(player->world_position(), 7, 3);
		temp.circleAoE->start(rect3dDrawManager);
		temp.cometEffect->initialize(player->world_position());
		temp.cometEffect->start(staticMeshDrawManager, rect3dDrawManager);
	}

	if (comets.size() >= 1) {
		auto& comet = comets.front();
		comet.cometEffect->debug_gui();
	}
	ImGui::End();
}

#endif // DEFERRED_RENDERING