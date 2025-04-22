#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <Engine/Module/Render/RenderPath/RenderPath.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/WorldManager.h>

#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>

#include <Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h>
#include <Engine/Module/DrawExecutor/LightingExecutor/PointLightingExecutor.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

class SceneGame final : public BaseScene {
public:
	void initialize() override;
	void update() override;
	void begin_rendering() override;
	void draw() const override;

private:
	std::unique_ptr<RenderPath> renderPath;
	std::unique_ptr<WorldManager> worldManager;
	std::unique_ptr<CollisionManager> collisionManager;

	std::unique_ptr<StaticMeshDrawManager> staticMeshDrawManager;
	std::unique_ptr<SkinningMeshDrawManager> skinningMeshDrawManager;
	std::unique_ptr<Rect3dDrawManager> rect3dDrawManager;
	std::unique_ptr<DirectionalLightingExecutor> directionalLightingExecutor;

	std::unique_ptr<DirectionalLightInstance> directionalLight;
};
