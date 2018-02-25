#include "stdafx.h"
#include "main_window.h"


LRESULT CALLBACK WndProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	//Send message to handler in System class
	return handle->MsgProc(hWnd, msg, wParam, lParam);
}


MainWindow::MainWindow()
	: hInstance()
	, hWnd()
	, appName()
{
	handle = this;
}


MainWindow::~MainWindow() {
}


bool MainWindow::InitWindow(LPCWSTR name, u32 width, u32 height) {
	appName      = name;
	window_width  = width;
	window_height = height;

	u32 xPos = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
	u32	yPos = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;

	hInstance = GetModuleHandle(NULL);


	//----------------------------------------------------------------------------------
	// Window class definition
	//----------------------------------------------------------------------------------
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);


	// Register window class
	if (!RegisterClassEx(&wc)) {
		OutputDebugString(L"Failed to create window class\n");
		return false;
	}

	// Create window and store window handle
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName, WS_OVERLAPPEDWINDOW,
							xPos, yPos, window_width, window_height, NULL, NULL, hInstance, NULL);
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
	MSG   msg = { 0 };
	bool done = false;

	// Main loop
	while (!done) {

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {

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