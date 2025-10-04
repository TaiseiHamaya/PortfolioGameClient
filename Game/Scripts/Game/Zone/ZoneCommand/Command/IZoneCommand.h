#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

class IZoneCommand {
public:
	IZoneCommand() = default;
	virtual ~IZoneCommand() = default;

	__CLASS_NON_COPYABLE(IZoneCommand)

public:
	virtual void execute() = 0;
};
