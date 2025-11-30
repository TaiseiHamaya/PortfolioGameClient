#pragma once

#include <Engine/Application/Framework.h>

#include <Library/Utility/Template/SingletonInterface.h>

class Portfolio final : public Framework {
private:
	class ProtoBufManager final : SingletonInterface<ProtoBufManager> {
		friend class Portfolio;

		__CLASS_SINGLETON_INTERFACE(ProtoBufManager)

	public:
		struct OnDestory {
			~OnDestory();
		} onDestroy;
	};

public:
	void initialize() override;
};

