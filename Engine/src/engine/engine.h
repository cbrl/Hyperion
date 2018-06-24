#pragma once

#include "engine/main_window.h"

#include "engine_util.h"
#include "ecs.h"
#include "sysmon/system_monitor.h"
#include "timer/timer.h"
#include "fps/fps.h"
#include "input.h"
#include "rendering_mgr.h"
#include "display/display_config.h"
#include "scene/scene.h"


class Engine final : public MainWindow {
public:
	Engine()
		: resizing(false) {
	};
	~Engine();

	bool init();

	void addSystems() const;

	// Unload the active scene (if applicable) and apply a new scene
	void loadScene(unique_ptr<Scene>&& new_scene);

	// Begin the main loop
	void run() override;

	LRESULT msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) override;

	void onResize(u32 window_width, u32 window_height);


	[[nodiscard]]
	HWND getHwnd() const {
		return hWnd;
	}

	[[nodiscard]]
	u32 getWindowWidth() const {
		return window_width;
	}

	[[nodiscard]]
	u32 getWindowHeight() const {
		return window_height;
	}

	[[nodiscard]]
	ECS& getECS() const {
		return *ecs_engine;
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
	void tick() const;
	void processInput() const;


private:
	unique_ptr<SystemMonitor> system_monitor;
	unique_ptr<HighResTimer> timer;
	unique_ptr<FPS> fps_counter;
	unique_ptr<Input> input;
	unique_ptr<RenderingMgr> rendering_mgr;
	unique_ptr<ECS> ecs_engine;
	unique_ptr<Scene> scene;

	bool resizing;
};
