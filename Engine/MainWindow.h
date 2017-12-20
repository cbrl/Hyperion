#pragma once

#include "System.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

using std::unique_ptr;
using std::make_unique;

class MainWindow {
	public:
		MainWindow();
		~MainWindow();

		bool Init();
		int Run();

		LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	private:
		bool InitWindow();


	private:
		unique_ptr<System> m_System;

		HINSTANCE m_hInstance;
		HWND      m_hWnd;

		bool m_Resizing;
};

static MainWindow *handle = nullptr;