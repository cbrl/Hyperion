#pragma once

#include "EngineUtil.h"
#include "MainWindow.h"
#include "Graphics.h"
#include "Timer.h"
#include "FPS.h"

#include <Windows.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

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
		unique_ptr<Graphics>   m_Graphics;
		unique_ptr<Timer>      m_Timer;
		unique_ptr<FPS>        m_FPSCounter;

		bool m_Resizing;
};
