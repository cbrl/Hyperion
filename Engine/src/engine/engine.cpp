#include "targetver.h"
#include "engine.h"
#include "imgui_message_forwarder.h"
#include "config/config_reader.h"
#include "config/config_writer.h"


#define CONFIG_FILE "./config.txt"


std::unique_ptr<Engine> SetupEngine() {

	// Create the console
	AllocateConsole();

	// Read the config file
	ConfigReader reader;
	reader.readConfig(CONFIG_FILE);

	// Create display configuration
	DisplayConfig display_config;
	const auto* width = reader.getConfigVar<u32>(ConfigTokens::width);
	const auto* height = reader.getConfigVar<u32>(ConfigTokens::height);
	const auto* refresh = reader.getConfigVar<u32>(ConfigTokens::refresh);
	if (width && height) {
		if (refresh)
			display_config.setNearestDisplayDesc({*width, *height}, *refresh);
		else
			display_config.setNearestDisplayDesc({*width, *height});
	}

	// Create rendering configuration
	RenderingConfig render_config;
	const auto* smap_res = reader.getConfigVar<u32>(ConfigTokens::shadowmap_res);
	if (smap_res) {
		render_config.setShadowMapRes(*smap_res);
	}

	// Create Engine
	const auto* title = reader.getConfigVar<std::string>(ConfigTokens::win_title);
	std::wstring window_title;
	if (title) {
		window_title = title->empty() ? L"Engine" : StrToWstr(*title);
	}
	else {
		window_title = L"Engine";
	}

	return std::make_unique<Engine>(window_title, std::move(display_config), std::move(render_config));
}



LRESULT EngineMessageHandler::msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {

		// Handle Alt-Enter keypress
		case WM_SYSKEYDOWN: {
			if (wParam == VK_RETURN) {
				on_fullscreen_toggle();
			}
			return 0;
		}

		case WM_SIZE: {
			on_resize();
			return 0;
		}

		default:
			return 0;
	}
}	



Engine::Engine(std::wstring title,
               DisplayConfig display_config,
               RenderingConfig rendering_config)
	: exit_requested(false)
	, resize_requested(false)
	, toggle_fullscreen(false) {

	init(std::move(title), std::move(display_config), std::move(rendering_config));
}

Engine::~Engine() {
	quit();
}


void Engine::quit() {

	// Write the current configuration values to a file
	ConfigWriter::write(*this, CONFIG_FILE);

	// Explicity delete the scene before the rendering manager.
	// This prevents D3D from potentially reporting live resources
	// that are going to be deleted right after the report.
	scene.reset();
}


void Engine::init(std::wstring title,
                  DisplayConfig display_config,
                  RenderingConfig rendering_config) {

	// Get the display resolution
	{
		size_t i = 0;
		for (const auto& mode : display_config.getDisplayDescList()) {
			const f32 rr = static_cast<f32>(mode.RefreshRate.Numerator) / static_cast<f32>(mode.RefreshRate.Denominator);
			std::cout << i++ << ": " << mode.Width << "x" << mode.Height << " " << rr << "Hz\n";
		}

		size_t select = std::numeric_limits<size_t>::max();
		while (select >= display_config.getDisplayDescList().size()) {
			std::cout << "\nSelect desired resolution \n>> ";
			std::cin >> select;
		}
		display_config.setDisplayDesc(select);
	}

	// Create the main window
	{
		auto win_config = std::make_shared<WindowConfig>(gsl::make_not_null(GetModuleHandle(nullptr)));
		window = std::make_unique<Window>(win_config,
		                                  title,
		                                  display_config.getDisplayResolution());

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

		window->addHandler(gsl::make_not_null(&msg_handler));
		window->addHandler(gsl::make_not_null(&InputMessageHandler::handler));
		window->addForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));

		Logger::log(LogLevel::info, "Initialized main window");
	}


	// System Monitor
	system_monitor = std::make_unique<SystemMonitor>();

	// Input Handler
	input = std::make_unique<Input>(gsl::make_not_null(window->getHandle()));

	// Timer
	timer = std::make_unique<HighResTimer>();

	// FPS Counter
	fps_counter = std::make_unique<FPS>();
	fps_counter->setWaitTime(250ms);

	// Rendering Manager
	rendering_mgr = std::make_unique<RenderingMgr>(gsl::make_not_null(window->getHandle()), std::move(display_config), std::move(rendering_config));
}


void Engine::loadScene(std::unique_ptr<Scene>&& new_scene) {
	
	if (scene) {
		const auto name = scene->getName();
		scene.reset();
		Logger::log(LogLevel::info, "Unloaded scene: {}", name);
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

	// F2: Toggle mouse mode
	if (input->isKeyPressed(Keyboard::F2)) {
		if (input->getMouseMode() == Mouse::MODE_ABSOLUTE) {
			window->removeForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
		}
		if (input->getMouseMode() == Mouse::MODE_RELATIVE) {
			window->removeForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
			window->addForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
		}
		input->toggleMouseMode();
	}

	// F3: Toggle mouse visibility (absolute mode only)
	if (input->isKeyPressed(Keyboard::F3)) {
		input->toggleMouseVisible();
	}
}
