#pragma once

#include "../IActionBasic.h"

/// <summary>
/// ジャンプ
/// </summary>
class JumpAction final : public IActionBasic {
public:
	void start() override;
	void update() override;
	void reset() override;

public:
	bool can_transition() const;
	bool end_action() const;
};
