#pragma once

#include <memory>
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "util\EngineUtil.h"
#include "system\MainWindow.h"
#include "direct3d\Direct3D.h"
#include "input\input.h"
#include "rendering\Renderer.h"
#include "rendering\RenderingMgr.h"
#include "scene\Scene.h"
#include "util\timer\Timer.h"
#include "util\fps\FPS.h"

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

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

		RenderingMgr* GetRenderingMgr() { return rendering_mgr.get(); }
		Scene* GetScene()               { return scene.get(); }
		Input* GetInput()               { return input.get(); }


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
