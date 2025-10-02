#pragma once

#include "../IActionBasic.h"

class JumpAction final : public IActionBasic {
public:
	void start() override;
	void update() override;
	void reset() override;

public:
	bool can_transition() const;
	bool end_action() const;
};
