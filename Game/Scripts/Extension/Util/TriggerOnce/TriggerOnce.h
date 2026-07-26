#pragma once

#include <functional>
#include <mutex>

/// <summary>
/// 一度限りのトリガー実行
/// 条件が満たされた時に一度だけ関数を呼び出す機構を提供
/// </summary>
class TriggerOnce {
public:
	TriggerOnce() = default;
	TriggerOnce(
		std::function<bool(void)> trigger_,
		std::function<void(void)> callFunction_
	);
	~TriggerOnce() = default;

	void update();
	void reset();
	void reset_trigger(std::function<bool(void)> trigger_);
	void set_call(std::function<bool(void)> callFunction_);

private:
	std::function<bool(void)> trigger;
	std::function<void(void)> callFunction;
	bool called{ false };
};
