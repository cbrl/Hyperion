#pragma once

#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "system\main_window.h"
#include "direct3d\direct3d.h"
#include "input\input.h"
#include "rendering\renderer.h"
#include "rendering\rendering_mgr.h"
#include "scene\scene.h"
#include "util\timer\timer.h"
#include "util\fps\fps.h"


#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 900

#define MSAA_STATE       false
#define VSYNC_STATE      false
#define FULLSCREEN_STATE false


class System : public MainWindow {
	public:
		static System* Get() { return system_ptr; }

		System();
		~System();

		bool Init();
		void Run();

		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void OnResize(int window_width, int window_height);

		HWND GetHWND()         const { return hWnd; }
		int  GetWindowWidth()  const { return window_width; }
		int  GetWindowHeight() const { return window_height; }

		RenderingMgr* GetRenderingMgr() const { return rendering_mgr.get(); }
		Scene*        GetScene()        const { return scene.get(); }
		Input*        GetInput()        const { return input.get(); }


	private:
		void Tick();


	private:
		static System* system_ptr;

		unique_ptr<MainWindow>   main_window;
		unique_ptr<RenderingMgr> rendering_mgr;
		unique_ptr<Scene>        scene;
		unique_ptr<Input>        input;
		unique_ptr<Timer>        timer;
		unique_ptr<FPS>          fps_counter;

		bool resizing;
};
