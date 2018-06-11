#pragma once

#include "os/windows.h"
#include "datatypes/datatypes.h"


class MainWindow {
public:
	MainWindow();
	virtual ~MainWindow() = default;

	bool initWindow(LPCWSTR name, u32 width, u32 height);
	virtual void run();

	virtual LRESULT msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);


protected:
	HINSTANCE hInstance;
	HWND hWnd;
	LPCWSTR app_name;
	u32 window_width;
	u32 window_height;
};


static MainWindow* handle = nullptr;
