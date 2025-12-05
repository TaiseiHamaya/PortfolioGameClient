#pragma once

#include <Engine/Runtime/Scene/Scene.h>

#include <list>

#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>
#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>
#include <Engine/Runtime/Input/InputHandler.h>

#include <Engine/Module/Render/RenderPipeline/Posteffect/GaussianBlur/GaussianBlurPipeline.h>

#include "Scripts/Extension/DrawExecutor/EnvironmentMeshExecutor.h"
#include "Scripts/Instance/MiscInstance/AOE/CircleAoe.h"
#include "Scripts/Instance/MiscInstance/Camera/FollowCamera.h"
#include "Scripts/Instance/MiscInstance/Effects/CometEffect.h"

#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>

struct CometAction {
	std::unique_ptr<CircleAoe> circleAoE;
	std::unique_ptr<CometEffect> cometEffect;
};

class LuminanceExtractionPSO;
class MargeTexture4PSO;
class BloomPipeline;
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

	GaussianBlurPipeline::Data blurData{
		.dispersion = 1.0f,
		.length = 40.0f,
		.sampleCount = 8
	};
};
