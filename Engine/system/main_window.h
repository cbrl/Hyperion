#pragma once

#include <Windows.h>
#include "util\engine_util.h"

// Forward declaration of parent class.
// Parent class reference is used to call Tick(), OnResize(), etc...
class System;

class MainWindow {
	public:
		MainWindow();
		~MainWindow();

		bool InitWindow(LPCWSTR name, int width, int height);
		virtual void Run();

		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	public:
		HINSTANCE hInstance;
		HWND      hWnd;
		LPCWSTR   appName;
		int       window_width;
		int       window_height;
};

static MainWindow *handle = nullptr;