#pragma once

#include <Engine/Module/Manager/SceneScript/ISceneScript.h>

#include <string>
#include <vector>

#include <Engine/Module/World/Mesh/Primitive/StringRectInstance.h>

class GameLogWindowManager final : public szg::ISceneScript {
public:
	enum class Type {
		ChatMessage,
		SystemMessage,
		ActionLog,
		DamagedLog,

		DebugInfo,

		Count,
	};

	struct LogBody {
		Type type;
		std::wstring message;
	};

public:
	GameLogWindowManager() = default;
	~GameLogWindowManager() = default;

	SZG_CLASS_MOVE_ONLY(GameLogWindowManager)

public:
	void initialize();

	void prev_update() override;

	void post_update() override;

public:
	void add_log(Type type, const std::wstring& log);

private:
	void update_log_window();

private:
	i32 index{ 0 };
	bool isChanged{ false };
	std::vector<LogBody> logs;

	constexpr static i32 MaxLogCount{ 10 };
	std::array<Reference<szg::StringRectInstance>, MaxLogCount> logInstances;

	std::array<Color4, static_cast<i32>(Type::Count)> typeColors{
		Color4{ 0xC6C6C6FF }, // ChatMessage
		Color4{ 0x663FE5FF }, // SystemMessage
		Color4{ 0xE6DB8AFF }, // DamagedLog
		Color4{ 0xE02929FF }, // ActionLog

		Color4{ 0xB630D8FF }, // DebugInfo
	};
};

