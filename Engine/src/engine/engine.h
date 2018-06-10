#pragma once

#include "engine/main_window.h"

#include "util/engine_util.h"
#include "datatypes/datatypes.h"
#include "sysmon/system_monitor.h"
#include "timer/timer.h"
#include "fps/fps.h"
#include "input/input.h"
#include "rendering/rendering_mgr.h"
#include "scene/scene.h"

#include "ecs.h"
#include "ecs_data/systems/systems.h"


#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 900

#define MSAA_STATE       false
#define VSYNC_STATE      false
#define FULLSCREEN_STATE false


class Engine final : public MainWindow {
public:
	Engine()
		: resizing(false) {
	};
	~Engine();

	bool init();
	void loadScene(unique_ptr<Scene>&& new_scene);
	void run() override;

	LRESULT msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) override;

	void onResize(u32 window_width, u32 window_height);

	HWND getHwnd() const { return hWnd; }
	u32 getWindowWidth() const { return window_width; }
	u32 getWindowHeight() const { return window_height; }

	ECS& getECS() const { return *ecs_engine; }
	Scene& getScene() const { return *scene; }
	Input& getInput() const { return *input; }
	Timer& getTimer() const { return *timer; }
	FPS& getFPSCounter() const { return *fps_counter; }
	SystemMonitor& getSysMon() const { return *system_monitor; }
	RenderingMgr& getRenderingMgr() const { return *rendering_mgr; }


private:
	void tick() const;
	void processInput() const;


private:
	unique_ptr<SystemMonitor> system_monitor;
	unique_ptr<Timer> timer;
	unique_ptr<FPS> fps_counter;
	unique_ptr<Input> input;
	unique_ptr<RenderingMgr> rendering_mgr;
	unique_ptr<ECS> ecs_engine;
	unique_ptr<Scene> scene;

	bool resizing;
};
