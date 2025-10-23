#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// ZoneCommandのインターフェース
/// </summary>
class IZoneCommand {
public:
	IZoneCommand() = default;
	virtual ~IZoneCommand() = default;

	__CLASS_NON_COPYABLE(IZoneCommand)

public:
	/// <summary>
	/// 実行
	/// </summary>
	virtual void execute() = 0;
};
