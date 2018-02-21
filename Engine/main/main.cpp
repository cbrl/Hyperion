#include "stdafx.h"
#include "util\engine_util.h"
#include "system\System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	// Create system object
	std::unique_ptr<System> system = make_unique<System>();

	// Initialize system
	if (!system->Init()) {
		return 1;
	}

	// Run
	system->Run();

	return 0;
}