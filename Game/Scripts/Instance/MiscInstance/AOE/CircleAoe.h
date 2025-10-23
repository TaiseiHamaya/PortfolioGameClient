#pragma once

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

class Rect3dDrawManager;

/// <summary>
/// 円形サークル
/// </summary>
class CircleAoe : public WorldInstance {
public:
	CircleAoe();
	~CircleAoe();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	/// <param name="radius"></param>
	/// <param name="duration_"></param>
	void initialize(const Vector3& position, float radius, float duration_);
	
	/// <summary>
	/// 開始処理
	/// </summary>
	/// <param name="rectDraw"></param>
	void start(Reference<Rect3dDrawManager> rectDraw);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="rectDraw"></param>
	void end(Reference<Rect3dDrawManager> rectDraw);

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

public:
	bool is_end() const;

private:
	WorldTimer timer;
	r32 duration{ 0.0f };

	std::unique_ptr<Rect3d> base;
	std::unique_ptr<Rect3d> effect;
};
