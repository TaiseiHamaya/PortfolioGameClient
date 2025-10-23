#pragma once

#include <memory>
#include <unordered_map>

#include <Engine/Module/World/WorldManager.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Instance/IEntity/IEntity.h"

class SkinningMeshDrawManager;
class Rect3dDrawManager;

/// <summary>
/// Zone上のエンティティ管理クラス
/// </summary>
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
	/// <summary>
	/// 追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="initjson"></param>
	/// <returns></returns>
	template<typename T>
		requires std::derived_from<T, IEntity>
	Reference<T> generate(const std::filesystem::path& initjson);

	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="serverId"></param>
	void destroy(u64 serverId);

	/// <summary>
	/// サーバーIDの取得
	/// </summary>
	/// <param name="id"></param>
	/// <returns>存在しない場合は参照なしReference</returns>
	Reference<IEntity> inquire_server_id(u64 id) const;

	/// <summary>
	/// クライアント用IDの取得
	/// </summary>
	/// <param name="localId"></param>
	/// <returns>存在しない場合は参照なしReference</returns>
	Reference<IEntity> inquire_local_id(u64 localId) const;

	/// <summary>
	/// 既存のエンティティに対し、サーバーIDの登録
	/// </summary>
	/// <param name="serverId"></param>
	/// <param name="entity"></param>
	void register_server_id(u64 serverId, Reference<IEntity> entity);

private:
	Reference<WorldManager> worldManager;
	Reference<SkinningMeshDrawManager> skinDraw;
	Reference<Rect3dDrawManager> rectDraw;

	std::unordered_map<u64, std::unique_ptr<IEntity>> entities;
	std::unordered_map<u64, Reference<IEntity>> entityRefByServerId;

	u64 localIdCounter{ 0 };
};

template<typename T>
	requires std::derived_from<T, IEntity>
inline Reference<T> EntityManager::generate(const std::filesystem::path& initJson) {
	std::unique_ptr<T> temp = worldManager->create<T>(nullptr);
	temp->initialize(initJson);
	temp->setup(skinDraw, rectDraw);
	Reference<T> result = temp;
	entities.emplace(localIdCounter, std::move(temp));
	++localIdCounter;

	return result;
}
