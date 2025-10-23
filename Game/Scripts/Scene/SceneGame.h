#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <list>
#include <vector>

#include <Library/Math/Vector3.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>
#include <Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>
#include <Engine/Module/Render/RenderNode/Deferred/DeferredAdaptor.h>
#include <Engine/Module/Render/RenderNode/Posteffect/RadialBlur/RadialBlurNode.h>
#include <Engine/Module/Render/RenderPath/RenderPath.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>
#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Input/InputHandler.h>

#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Manager/EntityManager.h"

#include "Scripts/Extension/DrawExecutor/EnvironmentMeshExecutor.h"
#include "Scripts/Extension/RenderNode/PostEffect/GaussianBlurNode.h"
#include "Scripts/Game/GameInputHandler.h"
#include "Scripts/Game/Zone/ZoneCommand/ZoneHandler.h"
#include "Scripts/Instance/MiscInstance/AOE/CircleAoe.h"
#include "Scripts/Instance/MiscInstance/Camera/FollowCamera.h"
#include "Scripts/Instance/MiscInstance/Effects/CometEffect.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Actions/PaladinHolySpirit.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Network/NetworkCluster.h"

struct CometAction {
	std::unique_ptr<CircleAoe> circleAoE;
	std::unique_ptr<CometEffect> cometEffect;
};

class LuminanceExtractionNode;
class MargeTextureNode;
class BloomNode;
class CubemapNode;

/// <summary>
/// メインゲームシーン
/// </summary>
class SceneGame final : public BaseScene {
public:
	void load() override;
	void initialize() override;
	void finalize() override;

	void begin() override;
	void update() override;
	void late_update() override;
	void begin_rendering() override;

	void draw() const override;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_update() override;
#endif // DEBUG_FEATURES_ENABLE


private:
	WorldTimer timer;

	// ---------- 描画パス関連 ----------
	std::unique_ptr<RenderPath> renderPath;
	// RenderTarget
	std::vector<RenderTexture> renderTextures;
	DeferredAdaptor::GBuffersType gBuffer;
	SingleRenderTarget baseRenderTexture;
	SingleRenderTarget radialBlurRenderTexture;
	SingleRenderTarget luminanceRenderTexture;
	SingleRenderTarget downSampleRenderTexture2;
	SingleRenderTarget downSampleRenderTexture4;
	SingleRenderTarget downSampleRenderTexture8;
	SingleRenderTarget downSampleRenderTexture16;
	SingleRenderTarget bloomBaseRenderTexture;
	// RenderNode
	std::shared_ptr<RadialBlurNode> radialBlurNode;
	std::shared_ptr<LuminanceExtractionNode> luminanceExtractionNode;
	std::shared_ptr<GaussianBlurNode> gaussianBlurNode2;
	std::shared_ptr<GaussianBlurNode> gaussianBlurNode4;
	std::shared_ptr<GaussianBlurNode> gaussianBlurNode8;
	std::shared_ptr<GaussianBlurNode> gaussianBlurNode16;
	std::shared_ptr<MargeTextureNode> margeTextureNode;
	std::shared_ptr<BloomNode> bloomNode;
	std::shared_ptr<CubemapNode> cubemapNode;

	// ---------- マネージャー関連 ----------
	std::unique_ptr<WorldManager> worldManager;
	// 描画管理
	std::unique_ptr<StaticMeshDrawManager> staticMeshDrawManager;
	std::unique_ptr<SkinningMeshDrawManager> skinningMeshDrawManager;
	std::unique_ptr<Rect3dDrawManager> rect3dDrawManager;
	std::unique_ptr<DirectionalLightingExecutor> directionalLightingExecutor;

	std::unique_ptr<EnvironmentMeshExecutor> environmentMeshExecutor;
	// その他
	std::unique_ptr<EntityManager> entityManager; // 全てのエンティティ管理
	std::unique_ptr<EffectManager> effectManager; // エフェクト管理
	std::unique_ptr<EnemyManager> enemyManager; // 敵管理

	std::unique_ptr<GameInputHandler> gameInputHandler; // プレイヤー入力
	NetworkCluster networkCluster; // ネットワーク
	ZoneHandler zoneHandler; // ゾーン処理

	// WorldInstance
	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::unique_ptr<FollowCamera> camera3D;
	std::unique_ptr<StaticMeshInstance> skydome;

	std::list<CometAction> comets;

	GaussianBlurNode::GaussianBlurInfo blurData{
		.dispersion = 1.0f,
		.length = 40.0f,
		.sampleCount = 8
	};
};
