#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <Engine/Module/Render/RenderPath/RenderPath.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/WorldManager.h>

#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>

#include <Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h>
#include <Engine/Module/DrawExecutor/Mesh/Primitive/Rect3dDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/SkinningMeshDrawManager.h>
#include <Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h>

#include "Scripts/Manager/EntityManager.h"

#include "Scripts/MiscInstance/Camera/FollowCamera.h"
#include "Scripts/MiscInstance/Enemy/EnemyManager.h"
#include "Scripts/Player/Player.h"
#include "Scripts/Player/LocalPlayerCommandHandler.h"

class SceneGame final : public BaseScene {
public:	void load() override;
	void initialize() override;

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
	std::unique_ptr<RenderPath> renderPath;
	std::unique_ptr<WorldManager> worldManager;

	std::unique_ptr<StaticMeshDrawManager> staticMeshDrawManager;
	std::unique_ptr<SkinningMeshDrawManager> skinningMeshDrawManager;
	std::unique_ptr<Rect3dDrawManager> rect3dDrawManager;
	std::unique_ptr<DirectionalLightingExecutor> directionalLightingExecutor;

	std::unique_ptr<EntityManager> entityManager;

	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::unique_ptr<FollowCamera> camera3D;
	std::unique_ptr<StaticMeshInstance> skydome;
	std::unique_ptr<EnemyManager> enemyManager;
	Reference<Player> player;

	std::unique_ptr<LocalPlayerCommandHandler> localPlayerCommandHandler;
};
