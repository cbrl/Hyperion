#include <memory>

#include "os/windows/windows.h"

import rendering;
import test_scene;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Enable run-time memory check for debug builds
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	// Create the engine
	std::unique_ptr<render::Engine> engine = SetupEngine();

	// Initialize the engine
	engine->loadScene(std::make_unique<TestScene>());

	// Run the engine
	engine->run();

	// Quit the engine
	engine.reset();

	return 0;
}
