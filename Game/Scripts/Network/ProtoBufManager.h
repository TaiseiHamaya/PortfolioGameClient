#pragma once

#include <Library/Utility/Template/SingletonInterface.h>

class ProtoBufManager final : SingletonInterface<ProtoBufManager> {
	friend class Portfolio;

	SZG_CLASS_SINGLETON(ProtoBufManager)

public:
	static void Allocate();

public:
	struct OnDestroy {
		~OnDestroy();
	} onDestroy;
};