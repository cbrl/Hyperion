#include "stdafx.h"
#include "MainWindow.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//Send message to handler in System class
	return handle->MsgProc(hWnd, msg, wParam, lParam);
}


MainWindow::MainWindow() {
	handle = this;
}


MainWindow::~MainWindow() {
}


bool MainWindow::InitWindow(LPCWSTR name, int width, int height) {
	appName = name;
	windowHeight = width;
	windowWidth = height;

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
	int	yPos = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

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
							xPos, yPos, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
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


int MainWindow::Run() {
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

	return 0;
}


LRESULT MainWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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