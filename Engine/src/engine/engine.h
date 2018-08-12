#pragma once

#include "os/windows/window.h"

#include "log/log.h"
#include "sysmon/system_monitor.h"
#include "timer/timer.h"
#include "fps/fps.h"
#include "input.h"
#include "rendering_mgr.h"


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
	LRESULT msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) override;


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::function<void()> on_fullscreen_toggle;
	std::function<void()> on_resize;
};




//----------------------------------------------------------------------------------
// Engine
//----------------------------------------------------------------------------------
class Engine final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Engine()
		: toggle_fullscreen(false) {
	}

	Engine(const Engine& engine) = delete;
	Engine(Engine&& engine) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Engine();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Engine& operator=(const Engine& engine) = delete;
	Engine& operator=(Engine&& engine) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Initialization
	//----------------------------------------------------------------------------------

	// Initialize the engine
	void init();

	// Begin the main loop
	void run();


	//----------------------------------------------------------------------------------
	// Member Functions - Window
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	HWND getWindow() const {
		return window->getWindow();
	}

	[[nodiscard]]
	vec2_u32 getWindowSize() const {
		return window->getClientSize();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scene
	//----------------------------------------------------------------------------------

	// Unload the active scene (if applicable) and apply a new scene
	void loadScene(unique_ptr<Scene>&& new_scene);

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


	//----------------------------------------------------------------------------------
	// Member Functions - Timer
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	HighResTimer& getTimer() {
		return *timer;
	}

	[[nodiscard]]
	const HighResTimer& getTimer() const {
		return *timer;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - FPS
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	FPS& getFPSCounter() {
		return *fps_counter;
	}

	[[nodiscard]]
	const FPS& getFPSCounter() const {
		return *fps_counter;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - System Monitor
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	SystemMonitor& getSysMon() {
		return *system_monitor;
	}

	[[nodiscard]]
	const SystemMonitor& getSysMon() const {
		return *system_monitor;
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
	void tick();
	void updateSystem();
	void updateRendering();
	void renderFrame();
	void processInput() const;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	EngineMessageHandler msg_handler;

	unique_ptr<Window> window;
	unique_ptr<SystemMonitor> system_monitor;
	unique_ptr<HighResTimer> timer;
	unique_ptr<FPS> fps_counter;
	unique_ptr<Input> input;
	unique_ptr<RenderingMgr> rendering_mgr;
	unique_ptr<Scene> scene;

	bool toggle_fullscreen;
};
