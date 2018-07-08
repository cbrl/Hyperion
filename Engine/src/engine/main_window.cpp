#include "stdafx.h"
#include "main_window.h"


LRESULT CALLBACK WndProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	// Send message to handler in MainWindow (or derived class)
	return handle->msgProc(hWnd, msg, wParam, lParam);
}


MainWindow::MainWindow()
	: hInstance()
	, hWnd()
	, app_name() {
	handle = this;
}


bool MainWindow::initWindow(LPCWSTR name, u32 width, u32 height) {

	// Create a rect for the adjusted window size
	RECT window_rect;
	window_rect.top    = 0;
	window_rect.left   = 0;
	window_rect.right  = width;
	window_rect.bottom = height;

	// Calculate the total window size with decorations included
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

	app_name      = name;
	window_width  = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	const u32 x_pos = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
	const u32 y_pos = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;

	hInstance = GetModuleHandle(nullptr);


	//----------------------------------------------------------------------------------
	// Window class definition
	//----------------------------------------------------------------------------------
	WNDCLASSEX wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = &WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = app_name;
	wc.cbSize        = sizeof(WNDCLASSEX);


	// Register window class
	if (!RegisterClassEx(&wc)) {
		OutputDebugString(L"Failed to create window class\n");
		return false;
	}

	// Create window and store window handle
	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
	                      app_name,
	                      app_name,
	                      WS_OVERLAPPEDWINDOW,
	                      x_pos,
	                      y_pos,
	                      window_width,
	                      window_height,
	                      nullptr,
	                      nullptr,
	                      hInstance,
	                      nullptr);
	if (!hWnd) {
		OutputDebugString(L"Failed to create main window\n");
		return false;
	}

	// Show window and set focus
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return true;
}


void MainWindow::run() {
	MSG  msg  = {nullptr};
	bool done = false;

	// Main loop
	while (!done) {

		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				done = true;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Process frame
		}
	}
}


LRESULT MainWindow::msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		// Send other messages to the default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
