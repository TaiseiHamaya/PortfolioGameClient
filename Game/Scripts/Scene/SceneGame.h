#pragma once

#include <Engine/Runtime/Scene/Scene.h>

#include <list>
#include <vector>

#include <Library/Math/Vector3.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>
#include <Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>
#include <Engine/Module/Render/RenderPath/RenderPath.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>
#include <Engine/Module/Manager/World/WorldRoot.h>
#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Input/InputHandler.h>

#include "Scripts/Manager/EffectManager.h"
#include "Scripts/Manager/EntityManager.h"

#include "Scripts/Extension/DrawExecutor/EnvironmentMeshExecutor.h"
#include "Scripts/Extension/RenderNode/PostEffect/GaussianBlurNode.h"
#include "Scripts/Game/GameInputHandler.h"
#include "Scripts/Game/Zone/ZoneHandler.h"
#include "Scripts/Instance/MiscInstance/AOE/CircleAoe.h"
#include "Scripts/Instance/MiscInstance/Camera/FollowCamera.h"
#include "Scripts/Instance/MiscInstance/Effects/CometEffect.h"
#include "Scripts/Instance/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Instance/Player/Actions/PaladinHolySpirit.h"
#include "Scripts/Instance/Player/Player.h"
#include "Scripts/Network/NetworkCluster.h"

#include <Engine/Module/DrawExecutor/Mesh/Primitive/StringRectDrawManager.h>
#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>

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
class SceneGame final : public Scene {
public:
	SceneGame() noexcept;

public:
	void custom_load_asset() override;

	void custom_setup() override;


private:
	WorldTimer timer;

	std::unique_ptr<EnvironmentMeshExecutor> environmentMeshExecutor;

	// WorldInstance
	Reference<DirectionalLightInstance> directionalLight;

	Reference<FollowCamera> camera3D;
	Reference<StaticMeshInstance> skydome;
	Reference<StringRectInstance> testString;

	std::list<CometAction> comets;

	Reference<CometEffect> cometEffect;

	GaussianBlurNode::GaussianBlurInfo blurData{
		.dispersion = 1.0f,
		.length = 40.0f,
		.sampleCount = 8
	};
};
