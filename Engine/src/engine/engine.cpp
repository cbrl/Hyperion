#include "targetver.h"
#include "engine.h"
#include "imgui_forwarder/imgui_message_forwarder.h"
#include "config/config_tokens.h"
#include "json/json.h"


#define CONFIG_FILE "./config.json"


std::unique_ptr<Engine> LoadConfig(const fs::path& config_file) {

	// Window title
	std::string title = "Engine";

	// Configuration classes
	render::DisplayConfig   display_config;
	render::RenderingConfig render_config;
	KeyConfig               key_config;

	// Try to process the config file
	json config;
	std::ifstream file{config_file};

	if (file) {
		// Read file contents into json object
		file >> config;
		file.close();

		// Load display configuration
		try {
			config.at(ConfigTokens::display_config).get_to(display_config);
		}
		catch (json::exception& e) {
			Logger::log(LogLevel::err, e.what());
		}

		// Load rendering configuration
		try {
			config.at(ConfigTokens::render_config).get_to(render_config);
		}
		catch (json::exception& e) {
			Logger::log(LogLevel::err, e.what());
		}

		// Load key configuration
		try {
			config.at(ConfigTokens::key_config).get_to(key_config);
		}
		catch (json::exception& e) {
			Logger::log(LogLevel::err, e.what());
		}

		// Get title
		if (config.contains(ConfigTokens::win_title))
			config[ConfigTokens::win_title].get_to(title);
	}
	
	auto engine = std::make_unique<Engine>(
		StrToWstr(title),
		std::move(display_config),
		std::move(render_config),
		std::move(key_config)
	);

	if (!file) {
		engine->saveConfig(); //try to create a config file if it couldn't be opened
	}
	return engine;
}


std::unique_ptr<Engine> SetupEngine() {

	// Create the console
	AllocateConsole();

	// Load the configuration file and create the engine
	return LoadConfig(CONFIG_FILE);
}



LRESULT EngineMessageHandler::msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {
		case WM_SYSKEYDOWN: {
			if (wParam == VK_RETURN) { //Handle Alt-Enter keypress
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
               render::DisplayConfig display_config,
               render::RenderingConfig rendering_config,
               KeyConfig key_config)
	: key_config(std::move(key_config)) {

	init(std::move(title),
	     std::move(display_config),
	     std::move(rendering_config));
}


Engine::~Engine() {
	quit();
}


void Engine::quit() {

	Logger::log(LogLevel::info, "Shutting down...");

	// Explicity delete the scene before the rendering manager.
	// This prevents D3D from potentially reporting live resources
	// that are going to be deleted right after the report.
	scene.reset();
	rendering_mgr.reset();
	input.reset();
	window.reset();

	// Uninitialize the COM library
	CoUninitialize();
}


void Engine::saveConfig() {

	// Open the file
	std::ofstream file(CONFIG_FILE, std::ofstream::trunc);
	if (!file) return;

	const auto& display   = rendering_mgr->getDisplayConfig();
	const auto& rendering = rendering_mgr->getRenderingConfig();

	// Serialize the configuration data
	json config;
	config[ConfigTokens::display_config] = display;
	config[ConfigTokens::render_config]  = rendering;
	config[ConfigTokens::win_title]      = WstrToStr(window->getWindowTitle());
	config[ConfigTokens::key_config]     = key_config;

	// Write the file
	file << std::setw(4) << config << std::endl;
	file.close();
}


void Engine::init(std::wstring title,
                  render::DisplayConfig display_config,
                  render::RenderingConfig rendering_config) {

	// Initialize the COM library
	ThrowIfFailed(CoInitializeEx(NULL, COINIT_MULTITHREADED),
	              "Failed to initialize the COM library");

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

			const u32_2 size = window->getClientSize();
			if (scene) {
				scene->sendEvent<render::events::WindowResizeEvent>(size);
			}
			Logger::log(LogLevel::info, "Window resized to {}x{}", size[0], size[1]);
		};

		window->addHandler(gsl::make_not_null(&msg_handler));
		window->addHandler(gsl::make_not_null(&InputMessageHandler::handler));
		window->addForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));

		Logger::log(LogLevel::info, "Initialized main window");
	}

	// Input Handler
	input = std::make_unique<Input>(gsl::make_not_null(window->getHandle()));

	// Rendering Manager
	rendering_mgr = std::make_unique<render::RenderingMgr>(
		gsl::make_not_null(window->getHandle()),
		std::move(display_config),
		std::move(rendering_config)
	);

	// Bind exit key if not bound
	bool save_config = false;
	if (key_config.bindIfNotBound("Exit", Keyboard::Escape)) {
		save_config = true;
	}
	if (key_config.bindIfNotBound("ToggleMouseMode", Keyboard::F1)) {
		save_config = true;
	}
	if (key_config.bindIfNotBound("ToggleMouse", Keyboard::F2)) {
		save_config = true;
	}
	if (save_config)
		saveConfig();
}


void Engine::loadScene(std::unique_ptr<render::Scene>&& new_scene) {
	
	if (scene) {
		const auto name = scene->getName();
		scene.reset();
		Logger::log(LogLevel::info, "Unloaded scene: {}", name);
	}

	scene = std::move(new_scene);

	if (scene) {
		Logger::log(LogLevel::info, "Loading scene: {}", scene->getName());
		scene->load(*this);
		Logger::log(LogLevel::info, "Scene loaded: {}", scene->getName());
		timer.reset();
	}
}


void Engine::run() {

	Logger::log(LogLevel::info, "Begin main loop");

	// Show the window
	window->show(SW_SHOWNORMAL);

	// Main loop
	MSG msg = {};
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
			if (input->isKeyDown(key_config.getKey("Exit"))) {
				requestExit();
			}
		}
	}

	Logger::log(LogLevel::info, "End main loop");
}


void Engine::tick() {

	updateSystem();
	updateRendering();
	renderFrame();
}


void Engine::updateSystem() {

	system_monitor.tick();
	timer.tick();
	input->tick();
}


void Engine::updateRendering() {

	auto& swap_chain = rendering_mgr->getSwapChain();
	const bool lost_mode = swap_chain.lostMode();

	if (resize_requested) {
		const bool fs = swap_chain.isFullscreen();
		if (fs) swap_chain.switchMode(true);
		window->resizeClient(rendering_mgr->getDisplayConfig().getDisplayResolution());
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
		rendering_mgr->render(*scene, timer.deltaTime());
	}

	// Present the frame
	rendering_mgr->endFrame();
}


void Engine::processInput() const {

	// F2: Toggle mouse mode
	if (input->isKeyPressed(key_config.getKey("ToggleMouseMode"))) {
		if (input->getMouseMode() == Mouse::MODE_ABSOLUTE) {
			window->removeForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
		}
		if (input->getMouseMode() == Mouse::MODE_RELATIVE) {
			window->removeForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
			window->addForwarder(gsl::make_not_null(&ImGuiMessageForwarder::forwarder));
			input->setMouseVisible(true);
		}
		input->toggleMouseMode();
	}

	// F3: Toggle mouse visibility (absolute mode only)
	if (input->isKeyPressed(key_config.getKey("ToggleMouse"))) {
		input->toggleMouseVisible();
	}
}
