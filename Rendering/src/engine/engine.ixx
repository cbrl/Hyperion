module;

#include <functional>
#include <memory>
#include <string>

#include "sysmon/system_monitor.h"
#include "time/stopwatch.h"
#include "input.h"
#include "key_config.h"

#include "rendering_forward_decs.h"

export module rendering:engine;

import log;
import window;
import :rendering_mgr;
import :scene;


namespace render {

//----------------------------------------------------------------------------------
// EngineMessageHandler
//----------------------------------------------------------------------------------
class EngineMessageHandler final : public MessageHandler {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EngineMessageHandler() noexcept = default;
	EngineMessageHandler(const EngineMessageHandler& handler) = default;
	EngineMessageHandler(EngineMessageHandler&& handler) = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EngineMessageHandler() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EngineMessageHandler& operator=(const EngineMessageHandler& handler) = default;
	EngineMessageHandler& operator=(EngineMessageHandler&& handler) = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	LRESULT msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) override;

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::function<void()> on_fullscreen_toggle;
	std::function<void()> on_resize;
};




//----------------------------------------------------------------------------------
// Engine
//----------------------------------------------------------------------------------
export class Engine final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Engine(std::wstring title,
	       DisplayConfig display_config,
	       RenderingConfig rendering_config,
	       KeyConfig key_config);

	Engine(const Engine& engine) = delete;
	Engine(Engine&& engine) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Engine();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Engine& operator=(const Engine& engine) = delete;
	Engine& operator=(Engine&& engine) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Run
	//----------------------------------------------------------------------------------

	// Begin the main loop
	void run();

	// Request that the engine quit upon completion of the current frame
	void requestExit() noexcept {
		Logger::log(LogLevel::info, "Exit requested");
		exit_requested = true;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Config
	//----------------------------------------------------------------------------------

	void saveConfig();


	//----------------------------------------------------------------------------------
	// Member Functions - Window
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Window& getWindow() {
		return *window;
	}

	[[nodiscard]]
	const Window& getWindow() const {
		return *window;
	}

	// Request a resize of the window and renderer
	void requestResize() noexcept {
		Logger::log(LogLevel::info, "Resize requested");
		resize_requested = true;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scene
	//----------------------------------------------------------------------------------

	// Unload the active scene (if applicable) and apply a new scene
	void loadScene(std::unique_ptr<Scene>&& new_scene);

	[[nodiscard]]
	Scene& getScene() {
		return *scene;
	}

	[[nodiscard]]
	const Scene& getScene() const {
		return *scene;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Input
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Input& getInput() {
		return *input;
	}

	[[nodiscard]]
	const Input& getInput() const {
		return *input;
	}

	[[nodiscard]]
	KeyConfig& getKeyConfig() {
		return key_config;
	}

	[[nodiscard]]
	const KeyConfig& getKeyConfig() const {
		return key_config;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Timer
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Stopwatch<>& getTimer() {
		return timer;
	}

	[[nodiscard]]
	const Stopwatch<>& getTimer() const {
		return timer;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - System Monitor
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	SystemMonitor& getSysMon() {
		return system_monitor;
	}

	[[nodiscard]]
	const SystemMonitor& getSysMon() const {
		return system_monitor;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rendering Manager
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	RenderingMgr& getRenderingMgr() {
		return *rendering_mgr;
	}

	[[nodiscard]]
	const RenderingMgr& getRenderingMgr() const {
		return *rendering_mgr;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Functions - Initialization
	//----------------------------------------------------------------------------------
	void init(std::wstring title,
	          DisplayConfig display_config,
	          RenderingConfig rendering_config);

	void quit();


	//----------------------------------------------------------------------------------
	// Member Functions - Per Frame
	//----------------------------------------------------------------------------------
	void tick();
	void updateSystem();
	void updateRendering();
	void renderFrame();
	void processInput() const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	EngineMessageHandler msg_handler;

	// Main window
	std::unique_ptr<Window> window;

	// Rendering
	std::unique_ptr<RenderingMgr> rendering_mgr;
	std::unique_ptr<Scene> scene;

	// Input
	std::unique_ptr<Input> input;
	KeyConfig key_config;

	// Utility
	Stopwatch<> timer;
	SystemMonitor system_monitor;

	bool exit_requested    = false;
	bool resize_requested  = false;
	bool toggle_fullscreen = false;
};


} //namespace engine

//----------------------------------------------------------------------------------
// Engine Setup Function
//----------------------------------------------------------------------------------
export std::unique_ptr<render::Engine> SetupEngine();
