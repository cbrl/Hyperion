#pragma once

#include <Windows.h>
#include "util\datatypes\datatypes.h"


class MainWindow {
	public:
		MainWindow();
		~MainWindow();

		bool InitWindow(LPCWSTR name, u32 width, u32 height);
		virtual void Run();

		virtual LRESULT MsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);


	protected:
		HINSTANCE hInstance;
		HWND      hWnd;
		LPCWSTR   app_name;
		u32       window_width;
		u32       window_height;
};

static MainWindow *handle = nullptr;