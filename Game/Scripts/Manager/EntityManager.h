#pragma once

#include <memory>
#include <unordered_map>

#include <Engine/Module/World/WorldManager.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Instance/IEntity/IEntity.h"

class SkinningMeshDrawManager;
class Rect3dDrawManager;

class EntityManager final {
public:
	EntityManager() = default;
	~EntityManager() = default;

	__CLASS_NON_COPYABLE(EntityManager)

public:
	void setup(Reference<WorldManager> worldManager_, Reference<SkinningMeshDrawManager> skinDraw_, Reference<Rect3dDrawManager> rectDraw_);

	void begin();
	void update();
	void late_update();

public:
	template<typename T>
		requires std::derived_from<T, IEntity>
	Reference<T> generate(u64 id, const std::filesystem::path& initjson);
	void destroy(u64 id);

	Reference<IEntity> inquire(u64 id) const;

private:
	Reference<WorldManager> worldManager;
	Reference<SkinningMeshDrawManager> skinDraw;
	Reference<Rect3dDrawManager> rectDraw;

	std::unordered_map<u64, std::unique_ptr<IEntity>> entities;
};

template<typename T>
	requires std::derived_from<T, IEntity>
inline Reference<T> EntityManager::generate(u64 id, const std::filesystem::path& initjson) {
	std::unique_ptr<T> temp = worldManager->create<T>(nullptr);
	temp->initialize(initjson);
	temp->setup(skinDraw, rectDraw);
	Reference<T> result = temp;
	entities.emplace(id, std::move(temp));

	return result;
}
