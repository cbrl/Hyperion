#include "os/windows/windows.h"
#include "engine/engine.h"
#include "scenes/test_scene.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Enable run-time memory check for debug builds
	// May incorrectly report a handful of 16 byte blocks from spdlog, but
	// the singleton pattern used in Logger seems to prevent this
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	// Create the engine
	unique_ptr<Engine> engine = make_unique<Engine>();

	// Initialize the engine
	if (!engine->init()) {
		return 1;
	}
	engine->loadScene(make_unique<TestScene>());

	// Run the engine
	engine->run();

	// Quit the engine
	engine.reset();

	return 0;
}
