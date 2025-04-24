#pragma once

#include <memory>
#include <unordered_map>

#include <Engine/Module/World/WorldManager.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/IEntity/BaseEntity.h"

class EntityManager final {
public:
	EntityManager() = default;
	~EntityManager() = default;

	__CLASS_NON_COPYABLE(EntityManager)

public:
	void start(Reference<WorldManager> worldManager_);

	void begin();
	void update();
	void late_update();

public:
	template<typename T>
	Reference<T> generate(u64 id);
	void destroy(u64 id);

	Reference<BaseEntity> get_entity(u64 id);

private:
	Reference<WorldManager> worldManager;
	std::unordered_map<u64, std::unique_ptr<BaseEntity>> entities;
};

template<typename T>
inline Reference<T> EntityManager::generate(u64 id) {
	std::unique_ptr<T> temp = worldManager->create<T>(nullptr);

	std::string typeName = typeid(T).name();
	if constexpr (std::is_class_v<T> || std::is_enum_v<T>) {
		typeName = (std::find(typeName.begin(), typeName.end(), ' ') + 1)._Ptr;
	}
	temp->initialize(typeName.c_str());
	Reference<T> result = temp;
	entities.emplace(id, std::move(temp));

	return result;
}
