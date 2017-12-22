#include "stdafx.h"
#include "EngineUtil.h"
#include "System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	std::unique_ptr<System> system = make_unique<System>();

	if (!system->Init()) {
		return 1;
	}

	return system->Run();
}