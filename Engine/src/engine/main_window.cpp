#include "stdafx.h"
#include "main_window.h"

#include "util/engine_util.h"


LRESULT CALLBACK WndProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	//Send message to handler in MainWindow (or derived class)
	return handle->MsgProc(hWnd, msg, wParam, lParam);
}


MainWindow::MainWindow()
	: hInstance()
	, hWnd()
	, app_name() {
	handle = this;
}


bool MainWindow::InitWindow(LPCWSTR name, u32 width, u32 height) {
	app_name = name;
	window_width = width;
	window_height = height;

	const u32 xPos = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
	const u32 yPos = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;

	hInstance = GetModuleHandle(nullptr);


	//----------------------------------------------------------------------------------
	// Window class definition
	//----------------------------------------------------------------------------------
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = app_name;
	wc.cbSize = sizeof(WNDCLASSEX);


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
	                      xPos,
	                      yPos,
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


void MainWindow::Run() {
	MSG msg = {nullptr};
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


LRESULT MainWindow::MsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

			// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
