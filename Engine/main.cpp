#include "stdafx.h"
#include "EngineUtil.h"
#include "MainWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	std::unique_ptr<MainWindow> window = make_unique<MainWindow>();

	if (!window->Init()) {
		return 1;
	}

	return window->Run();
}