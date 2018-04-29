#pragma once

#include <Windows.h>

#include "system\main_window.h"

#include "util\datatypes\datatypes.h"
#include "util\sysmon\system_monitor.h"
#include "util\timer\timer.h"
#include "util\fps\fps.h"
#include "input\input.h"
#include "rendering\rendering_mgr.h"
#include "scene\scene.h"
#include "scene\test_scene.h"


#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 900

#define MSAA_STATE       false
#define VSYNC_STATE      false
#define FULLSCREEN_STATE false


class System final : public MainWindow {
	public:
		System() = default;
		~System() = default;

		bool Init();
		void Run();

		LRESULT MsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

		void OnResize(u32 window_width, u32 window_height);

		HWND GetHWND()         const { return hWnd; }
		u32  GetWindowWidth()  const { return window_width; }
		u32  GetWindowHeight() const { return window_height; }

		Scene&         GetScene()              { return *scene; }
		Input&         GetInput()        const { return *input; }
		Timer&         GetTimer()        const { return *timer; }
		FPS&           GetFPSCounter()   const { return *fps_counter; }
		SystemMonitor& GetSysMon()       const { return *system_monitor; }
		RenderingMgr&  GetRenderingMgr() const { return *rendering_mgr; }
		

	private:
		void Tick();
		void ProcessInput();


	private:
		unique_ptr<Input>         input;
		unique_ptr<SystemMonitor> system_monitor;
		unique_ptr<Timer>         timer;
		unique_ptr<FPS>           fps_counter;
		unique_ptr<RenderingMgr>  rendering_mgr;
		unique_ptr<Scene>         scene;

		bool resizing;
};
