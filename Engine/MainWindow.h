#pragma once

#include "System.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

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
		System *m_System;

		HINSTANCE m_hInstance;
		HWND      m_hWnd;

		bool m_Resizing;
};

static MainWindow *handle = nullptr;