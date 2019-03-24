#include "targetver.h"
#include "engine.h"
#include "imgui_forwarder/imgui_message_forwarder.h"
#include "config/config_tokens.h"

#include "json/json.hpp"
using json = nlohmann::json;


#define CONFIG_FILE "./config.json"


const json default_config = {
	{ConfigTokens::width, 1200},
	{ConfigTokens::height, 900},
	{ConfigTokens::refresh, 60},
	{ConfigTokens::vsync, false},
	{ConfigTokens::fullscreen, false},
	{ConfigTokens::shadowmap_res, 512},
	{ConfigTokens::win_title, "Engine"},
};


// Can throw json::type_error
std::unique_ptr<Engine> LoadConfig(const json& config) {

	// Create display configuration
	DisplayConfig display_config;
	const auto width   = config[ConfigTokens::width].get<u32>();
	const auto height  = config[ConfigTokens::height].get<u32>();
	const auto refresh = config[ConfigTokens::refresh].get<u32>();
	display_config.setNearestDisplayDesc({width, height}, refresh);

	// Create rendering configuration
	RenderingConfig render_config;
	const auto smap_res = config[ConfigTokens::shadowmap_res].get<u32>();
	render_config.setShadowMapRes(smap_res);

	// Create Engine
	const auto title = config[ConfigTokens::win_title].get<std::string>();
	return std::make_unique<Engine>(StrToWstr(title), std::move(display_config), std::move(render_config));
}


std::unique_ptr<Engine> SetupEngine() {

	// Create the console
	AllocateConsole();

	// Read the config file, or load the default config if that fails
	json config;
	std::ifstream file{CONFIG_FILE};
	if (file) {
		file >> config;
		file.close();
	}
	else {
		return LoadConfig(default_config);
	}


	// Try to process the config file, or use the default config if that fails
	try {
		return LoadConfig(config);
	}
	catch (json::type_error& e) {
		Logger::log(LogLevel::err, e.what());
		return LoadConfig(default_config);
	}
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
	std::ofstream file(CONFIG_FILE, std::ofstream::trunc);
	if (!file) return;

	const auto& display       = rendering_mgr->getDisplayConfig();
	const auto& rendering     = rendering_mgr->getRenderingConfig();
		
	const auto title      = WstrToStr(window->getWindowTitle());
	const auto res        = window->getClientSize();
	const auto refresh    = display.getRoundedDisplayRefreshRate();
	const auto vsync      = display.isVsync();
	const auto fullscreen = display.isFullscreen();
	const auto shadowmap  = rendering.getShadowMapRes();

	json config;
	config[ConfigTokens::width]         = res[0];
	config[ConfigTokens::height]        = res[1];
	config[ConfigTokens::refresh]       = refresh;
	config[ConfigTokens::vsync]         = vsync;
	config[ConfigTokens::win_title]     = title;
	config[ConfigTokens::shadowmap_res] = shadowmap;

	file << std::setw(4) << config << std::endl;
	file.close();
}


void Engine::init(std::wstring title,
                  DisplayConfig display_config,
                  RenderingConfig rendering_config) {

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
				scene->sendEvent<WindowResizeEvent>(size);
			}
			Logger::log(LogLevel::info, "Window resized to {}x{}", size[0], size[1]);
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

	// Rendering Manager
	rendering_mgr = std::make_unique<RenderingMgr>(gsl::make_not_null(window->getHandle()), std::move(display_config), std::move(rendering_config));

	// Initialize the COM library
	auto com_result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ThrowIfFailed(com_result != S_FALSE, "Failed to initialize the COM library");
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
		Logger::log(LogLevel::info, "Scene loaded: {}", scene->getName());
		timer->reset();
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
			if (input->isKeyDown(Keyboard::Escape)) {
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

	system_monitor->tick();
	timer->tick();
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
		rendering_mgr->render(*scene, static_cast<f32>(timer->deltaTime()));
	}

	// Present the frame
	rendering_mgr->endFrame();
}


void Engine::processInput() const {

	// F2: Toggle mouse mode
	if (input->isKeyPressed(Keyboard::F1)) {
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
	if (input->isKeyPressed(Keyboard::F2)) {
		input->toggleMouseVisible();
	}
}
