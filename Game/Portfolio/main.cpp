#include <windows.h>

#include <memory>

#include "Portfolio/Portfolio.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Framework> framework =
		std::make_unique<Portfolio>();

	framework->run();

	framework.reset();

	return 0;
}

