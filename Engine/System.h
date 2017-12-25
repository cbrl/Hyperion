#pragma once

#include "EngineUtil.h"
#include "MainWindow.h"
#include "Direct3D.h"
#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"
#include "FPS.h"

#include <Windows.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define MSAA_STATE       false
#define VSYNC_STATE      false
#define FULLSCREEN_STATE false

using std::unique_ptr;
using std::make_unique;

class System : public MainWindow {
	public:
		System();
		~System();

		bool Init();
		int  Run();
		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnResize(int windowWidth, int windowHeight);


	private:
		unique_ptr<MainWindow> m_MainWindow;
		unique_ptr<Direct3D>   m_Direct3D;
		unique_ptr<Renderer>   m_Renderer;
		unique_ptr<Scene>      m_Scene;
		unique_ptr<Timer>      m_Timer;
		unique_ptr<FPS>        m_FPSCounter;

		bool m_Resizing;
};
