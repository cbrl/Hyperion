#include "targetver.h"
#include "engine.h"
#include "systems/systems.h"
#include "imgui_message_forwarder.h"


LRESULT EngineMessageHandler::msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {

		// Handle Alt-Enter keypress
		case WM_SYSKEYDOWN: {
			if (wParam == VK_RETURN) {
				on_fullscreen_toggle();
			}
		}

		case WM_SIZE: {
			on_resize();
		}

		default:
			return 0;
	}
}	



Engine::~Engine() {

	Logger::logFile(LogLevel::info, "<==========================END==========================>\n");

	// Explicity delete the scene before the rendering manager.
	// This prevents D3D from potentially reporting live resources
	// that are going to be deleted right after the report.
	scene.reset();
}


void Engine::init() {

	// Create the console
	AllocateConsole();

	Logger::logFile(LogLevel::info, "<=========================START=========================>");


	// Create the display configuration
	DisplayConfig display_config(AAType::None, false, false);

	// Get the display resolution
	{
		size_t i = 0;
		for (const auto& mode : display_config.getDisplayDescList()) {
			const f32 rr = static_cast<f32>(mode.RefreshRate.Numerator) / static_cast<f32>(mode.RefreshRate.Denominator);
			std::cout << i++ << ": " << mode.Width << "x" << mode.Height << " " << rr << "Hz\n";
		}

		std::cout << "Select desired resolution:\n";
		size_t select = 0;
		std::cin >> select;
		display_config.setDisplayDesc(select);
	}

	// Create the main window
	{
		auto win_config = std::make_shared<WindowConfig>(GetModuleHandle(nullptr), L"Engine");
		window = std::make_unique<Window>(win_config,
		                                  L"Engine",
		                                  vec2_u32{ display_config.getDisplayWidth(),
		                                            display_config.getDisplayHeight() });

		msg_handler.on_fullscreen_toggle = [this]() {
			toggle_fullscreen = true;
		};

		msg_handler.on_resize = [this]() {
			if (!rendering_mgr) return;
			rendering_mgr->onResize();

			const vec2_u32 size = window->getClientSize();
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
	system_monitor = std::make_unique<SystemMonitor>();

	// Input Handler
	input = std::make_unique<Input>(window->getWindow());

	// Timer
	timer = std::make_unique<HighResTimer>();

	// FPS Counter
	fps_counter = std::make_unique<FPS>();

	// Rendering Manager
	rendering_mgr = std::make_unique<RenderingMgr>(window->getWindow(), display_config);
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

	// Show the window
	window->show(SW_SHOWNORMAL);

	// Main loop
	MSG  msg  = {};
	while (msg.message != WM_QUIT && !exit_requested) {

		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {
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
				requestExit();
			}
		}
	}
}


void Engine::tick() {

	updateSystem();
	updateRendering();
	renderFrame();
}


void Engine::updateSystem() {

	system_monitor->tick();
	timer->tick();
	fps_counter->tick();
	input->tick();
}


void Engine::updateRendering() {

	auto& swap_chain = rendering_mgr->getSwapChain();
	const bool lost_mode = swap_chain.lostMode();

	if (resize_requested) {
		const bool fs = swap_chain.isFullscreen();
		if (fs) swap_chain.switchMode(true);
		window->resizeWindow(rendering_mgr->getDisplayConfig().getDisplayResolution());
		if (fs) swap_chain.switchMode(true);
		resize_requested = false;
	}

	if (lost_mode || toggle_fullscreen) {
		swap_chain.switchMode(!lost_mode);
		toggle_fullscreen = false;
		Logger::log(LogLevel::info, "Fullscreen: {}", swap_chain.isFullscreen());
	}
}


void Engine::renderFrame() {

	// Begin a new frame
	rendering_mgr->beginFrame();

	// Draw a frame
	if (scene) {
		scene->tick(*this);
		rendering_mgr->render(*scene);
	}

	// Present the frame
	rendering_mgr->endFrame();
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
