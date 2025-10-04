#pragma once

#include "../IActionBasic.h"

class IdleAction final : public IActionBasic {
public:
	IdleAction();
	~IdleAction() = default;

public:
	void start() override;

	void update() override;

	void reset() override;

public:
	r32 progress() const;
	bool can_transition() const;
	bool end_action() const;
};
