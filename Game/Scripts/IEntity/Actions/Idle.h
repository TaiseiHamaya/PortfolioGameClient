#pragma once

#include "../IActionBasic.h"

class Idle final : public IActionBasic {
public:
	void update();

	void reset();

public:
	virtual r32 progress() const;
	virtual bool can_transition() const;
	virtual bool end_action() const;
};
