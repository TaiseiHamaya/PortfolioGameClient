#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// ZoneCommandのインターフェース
/// </summary>
class IZoneCommand {
public:
	IZoneCommand() = default;
	virtual ~IZoneCommand() = default;

	SZG_CLASS_MOVE_ONLY(IZoneCommand)

public:
	/// <summary>
	/// 実行
	/// </summary>
	virtual void execute() = 0;
};
