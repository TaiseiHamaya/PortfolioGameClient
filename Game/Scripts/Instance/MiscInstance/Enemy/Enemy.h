#pragma once

#include <memory>

#include "Scripts/Instance/IEntity/IEntity.h"

class SphereCollider;

/// <summary>
/// 敵キャラクターのインターフェース
/// </summary>
class Enemy : public IEntity {
public:
	void move_to(const std::chrono::steady_clock::time_point& time, const Vector3& position) override;
};
