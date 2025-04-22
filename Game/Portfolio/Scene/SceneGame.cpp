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

#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>

void SceneGame::initialize() {
	// WorldManager
	worldManager = eps::CreateUnique<WorldManager>();
	collisionManager = std::make_unique<CollisionManager>();

	// DrawManager
	skinningMeshDrawManager = eps::CreateUnique<SkinningMeshDrawManager>();
	skinningMeshDrawManager->initialize(1);

	staticMeshDrawManager = eps::CreateUnique<StaticMeshDrawManager>();
	staticMeshDrawManager->initialize(1);

	rect3dDrawManager = eps::CreateUnique<Rect3dDrawManager>();
	rect3dDrawManager->initialize(1);
	rect3dDrawManager->make_instancing(0, 100);

	directionalLightingExecutor = eps::CreateUnique<DirectionalLightingExecutor>(1);


	// WorldInstances
	directionalLight = worldManager->create<DirectionalLightInstance>();

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
}

void SceneGame::update() {
}

void SceneGame::begin_rendering() {

	worldManager->update_matrix();

	//particleEmitter->transfer();
	directionalLight->transfer();
	//pointLight->transfer();
	//camera3D->transfer();

	directionalLightingExecutor->begin();
	directionalLightingExecutor->write_to_buffer(directionalLight);
	rect3dDrawManager->transfer();
	staticMeshDrawManager->transfer();
	skinningMeshDrawManager->transfer();
}

void SceneGame::draw() const {
	// 3DMesh
	renderPath->begin();
	//camera3D->register_world_projection(2);
	//staticMeshDrawManager->draw_layer(0);

	// SkinMesh
	renderPath->next();
	//camera3D->register_world_projection(2);
	//skinningMeshDrawManager->draw_layer(0);

	// NonLightingPixel
	renderPath->next();

	// DirectionalLight
	renderPath->next();
	//camera3D->register_world_lighting(1);
	//directionalLightingExecutor->draw_command();

	// ParticleBillboard
	renderPath->next();
	//camera3D->register_world_projection(1);
	////particleEmitter->draw();

	// Rect3D
	renderPath->next();
	//camera3D->register_world_projection(3);
	//camera3D->register_world_lighting(4);
	//directionalLightingExecutor->set_command(5);
	//rect3dDrawManager->draw_layer(0);

	renderPath->next();

#ifdef DEBUG_FEATURES_ENABLE
	// LineDraw
	//camera3D->register_world_projection(1);
	//collisionManager->debug_draw3d();
	//camera3D->debug_draw_frustum();

	renderPath->next();
#endif // DEFERRED_RENDERING
}
