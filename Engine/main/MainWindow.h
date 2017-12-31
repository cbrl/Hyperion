#pragma once

#include <Windows.h>
#include "util\EngineUtil.h"

// Forward declaration of parent class.
// Parent class reference is used to call Tick(), OnResize(), etc...
class System;

class MainWindow {
	public:
		MainWindow();
		~MainWindow();

		bool InitWindow(LPCWSTR name, int width, int height);
		virtual int Run();

		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	public:
		HINSTANCE m_hInstance;
		HWND      m_hWnd;
		LPCWSTR   m_AppName;
		int       m_WindowWidth;
		int       m_WindowHeight;
};

static MainWindow *handle = nullptr;