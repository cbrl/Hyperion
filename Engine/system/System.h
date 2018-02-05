#pragma once

#include <memory>
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "util\EngineUtil.h"
#include "main\MainWindow.h"
#include "direct3d\Direct3D.h"
#include "input\input.h"
#include "rendering\Renderer.h"
#include "rendering\RenderingMgr.h"
#include "scene\Scene.h"
#include "util\timer\Timer.h"
#include "util\fps\FPS.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define MSAA_STATE       false
#define VSYNC_STATE      false
#define FULLSCREEN_STATE false

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

class System : public MainWindow {
	public:
		static System* Get() { return systemPtr; }

		System();
		~System();

		bool Init();
		int  Run();

		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnResize(int windowWidth, int windowHeight);

		RenderingMgr* GetRenderingMgr() { return renderingMgr.get(); }
		Scene* GetScene()               { return scene.get(); }
		Input* GetInput()               { return input.get(); }


	private:
		void Tick();


	private:
		static System* systemPtr;

		unique_ptr<MainWindow>   mainWindow;
		unique_ptr<RenderingMgr> renderingMgr;
		unique_ptr<Scene>        scene;
		unique_ptr<Input>        input;
		unique_ptr<Timer>        timer;
		unique_ptr<FPS>          fpsCounter;

		bool m_Resizing;
};
