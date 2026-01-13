#pragma once

#include <Engine/Application/Framework.h>

#include <Library/Utility/Template/SingletonInterface.h>

class Portfolio final : public szg::Framework {
private:
	class ProtoBufManager final : SingletonInterface<ProtoBufManager> {
		friend class Portfolio;

		SZG_CLASS_SINGLETON(ProtoBufManager)

	public:
		struct OnDestroy {
			~OnDestroy();
		} onDestroy;
	};

public:
	void initialize() override;
};

