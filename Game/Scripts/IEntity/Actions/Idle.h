#pragma once

#include "../BaseAction.h"

class Idle final : public BaseAction {
public:
	void update();

	void reset();

public:
	virtual r32 progress() const;
	virtual bool can_transition() const;
	virtual bool end_action() const;
};
