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

	Engine() = default;
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
	// Member Functions
	//----------------------------------------------------------------------------------

	// Initialize the engine
	void init();

	// Unload the active scene (if applicable) and apply a new scene
	void loadScene(unique_ptr<Scene>&& new_scene);

	// Begin the main loop
	void run();

	[[nodiscard]]
	HWND getWindow() const {
		return window->getWindow();
	}

	[[nodiscard]]
	vec2_u32 getWindowSize() const {
		return window->getClientSize();
	}

	[[nodiscard]]
	Scene& getScene() const {
		return *scene;
	}

	[[nodiscard]]
	Input& getInput() const {
		return *input;
	}

	[[nodiscard]]
	HighResTimer& getTimer() const {
		return *timer;
	}

	[[nodiscard]]
	FPS& getFPSCounter() const {
		return *fps_counter;
	}

	[[nodiscard]]
	SystemMonitor& getSysMon() const {
		return *system_monitor;
	}

	[[nodiscard]]
	RenderingMgr& getRenderingMgr() const {
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
