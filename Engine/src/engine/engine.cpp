#include "stdafx.h"
#include "engine.h"
#include "systems/systems.h"
#include "imgui_message_forwarder.h"


LRESULT EngineMessageHandler::msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {

		// Handle window resize
		case WM_SIZE: {
			if (wParam == SIZE_MAXIMIZED) {
				on_resize();
			}
			else if (wParam == SIZE_RESTORED) {
				// Do nothing if resizing. Constantly calling the resize function would be slow.
				if (!resizing) {
					on_resize();
				}
			}

			return 0;
		}

		case WM_ENTERSIZEMOVE:
			resizing = true;
			return 0;

		case WM_EXITSIZEMOVE: {
			resizing = false;
			on_resize();
			return 0;
		}

		// Limit min window size
		case WM_GETMINMAXINFO:
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 480;
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 480;
			return 0;

		default:
			return 0;
	}
}	



Engine::~Engine() {

	Logger::logFile(LogLevel::info, "<==========================END==========================>\n");

	// Explicity delete the scene and entity component system before
	// the rendering manager. This prevents D3D from reporting live
	// resources that are going to be deleted right after the report.
	scene.reset();
	//ecs_engine.reset();
}


bool Engine::init() {

	// Create the console
	AllocateConsole();

	Logger::logFile(LogLevel::info, "<=========================START=========================>");


	// Create the display configuration
	DisplayConfig display_config(AAType::none, false, false);

	// Create the main window
	{
		auto win_config = make_shared<WindowConfig>(GetModuleHandle(nullptr), L"Engine");
		window = make_unique<Window>(win_config,
									 L"Engine",
									 vec2_u32{ display_config.getDisplayDesc().Width,
									           display_config.getDisplayDesc().Height });

		msg_handler.on_resize = [this]() {
			if (!rendering_mgr) return;

			const vec2_u32 size = window->getClientSize();
			rendering_mgr->resizeBuffers(size.x, size.y);

			if (scene) {
				scene->onResize(size);
			}

			Logger::log(LogLevel::info, "Viewport resized to {}x{}", size.x, size.y);
		};

		window->addHandler(&msg_handler);
		window->addHandler(&InputMessageHandler::handler);
		window->addForwarder(&ImGuiMessageForwarder::forwarder);

		Logger::log(LogLevel::info, "Initialized main window");
	}


	// System Monitor
	system_monitor = make_unique<SystemMonitor>();

	// Input Handler
	input = make_unique<Input>(window->getWindow());

	// Timer
	timer = make_unique<HighResTimer>();

	// FPS Counter
	fps_counter = make_unique<FPS>();

	// Rendering Manager
	rendering_mgr = make_unique<RenderingMgr>(window->getWindow(), display_config);


	return true;
}


void Engine::loadScene(unique_ptr<Scene>&& new_scene) {
	
	if (scene) {
		scene.reset();
		Logger::log(LogLevel::info, "Unloaded scene: {}", scene->getName());
	}

	scene = std::move(new_scene);

	if (scene) {
		Logger::log(LogLevel::info, "Loading scene: {}", scene->getName());
		scene->load(*this);
		Logger::log(LogLevel::info, "Finished loading scene: {}", scene->getName());
		timer->reset();
	}
}


void Engine::run() {
	window->show(SW_SHOWNORMAL);

	MSG  msg  = {};
	bool done = false;

	// Main loop
	while (!done) {

		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				done = true;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Process frame
			tick();

			// Process input
			processInput();

			// Quit if escape is pressed
			if (input->isKeyDown(Keyboard::Escape)) {
				done = true;
			}
		}
	}
}


void Engine::tick() const {

	// Update system metrics
	system_monitor->tick();
	timer->tick();
	fps_counter->tick();

	// Update the input state
	input->tick();

	// Update the scene
	scene->tick(*this);

	// Render the scene
	rendering_mgr->render(*this);
}


void Engine::processInput() const {

	// F1: Toggle mouse mode
	if (input->isKeyPressed(Keyboard::F1)) {
		if (input->getMouseMode() == Mouse::MODE_ABSOLUTE) {
			window->removeForwarder(&ImGuiMessageForwarder::forwarder);
		}
		if (input->getMouseMode() == Mouse::MODE_RELATIVE) {
			window->removeForwarder(&ImGuiMessageForwarder::forwarder);
			window->addForwarder(&ImGuiMessageForwarder::forwarder);
		}
		input->toggleMouseMode();
	}

	// F2: Toggle mouse visibility (absolute mode only)
	if (input->isKeyPressed(Keyboard::F2)) {
		input->toggleMouseVisible();
	}
}
