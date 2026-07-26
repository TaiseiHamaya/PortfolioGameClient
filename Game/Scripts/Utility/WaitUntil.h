#pragma once

#include <functional>

#include <Engine/Module/World/Mesh/Primitive/Rect3d.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

/// <summary>
/// 条件待機管理クラス
/// 指定された条件が満たされるまで待機するフローを管理
/// </summary>
class WaitUntil {
public:
	void initialize(Reference<szg::Rect3d> rect_);
	void reset(std::function<bool()> condition_);

	void end_force();

	bool update();

	const szg::WorldTimer& timer_imm() const { return timer; }

private:
	std::function<bool()> condition;

	szg::WorldTimer timer;
	Reference<szg::Rect3d> rect;
};
