#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <unordered_map>

#include <Engine/Module/Manager/World/WorldRoot.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Scripts/Instance/IEntity/IEntity.h"

namespace szg {

class SkinningMeshDrawManager;
class StringRectDrawManager;
class Rect3dDrawManager;

} // namespace szg

/// <summary>
/// Zone上のエンティティ管理クラス
/// </summary>
class EntityManager final : public szg::ISceneScript {
public:
	EntityManager() = default;
	~EntityManager() = default;

	SZG_CLASS_MOVE_ONLY(EntityManager)

public:
	void prev_update() override;

	void post_update() override;

	void setup(Reference<szg::WorldRoot> worldRoot);

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
	Reference<szg::WorldRoot> worldRoot;

	std::unordered_map<u64, Reference<IEntity>> entities;
	std::unordered_map<u64, Reference<IEntity>> entityRefByServerId;

	std::unordered_set<u64> removedEntityIds;

	u64 localIdCounter{ 0 };
};

template<typename T>
	requires std::derived_from<T, IEntity>
inline Reference<T> EntityManager::generate(const std::filesystem::path& initJson) {
	Reference<T> temp = worldRoot->instantiate<T>(nullptr);
	temp->initialize(initJson, localIdCounter);
	entities.emplace(localIdCounter, temp);
	++localIdCounter;

	return temp;
}
