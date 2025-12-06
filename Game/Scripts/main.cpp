#include <windows.h>

#include <memory>

#include "Scripts/Portfolio.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<szg::Framework> framework =
		std::make_unique<Portfolio>();

	framework->run();

	framework.reset();

	return 0;
}

