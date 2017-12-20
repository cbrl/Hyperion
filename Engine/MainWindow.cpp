#include "stdafx.h"
#include "MainWindow.h"

#include <Windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//Send message to handler in System class
	return handle->MsgProc(hWnd, msg, wParam, lParam);
}


MainWindow::MainWindow() : m_System(nullptr) {
	handle = this;
}


MainWindow::~MainWindow() {
}


bool MainWindow::Init() {
	if (!InitWindow()) {
		return false;
	}

	m_System = make_unique<System>(m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!m_System->Init()) {
		return false;
	}

	return true;
}


bool MainWindow::InitWindow() {
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2;
	int	yPos = (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2;

	m_hInstance = GetModuleHandle(NULL);
	LPCWSTR appName = L"Engine";

	//----------------------------------------------------------------------------------
	// Window class definition
	//----------------------------------------------------------------------------------
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
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
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName, WS_OVERLAPPEDWINDOW,
							xPos, yPos, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, m_hInstance, NULL);
	if (!m_hWnd) {
		OutputDebugString(L"Failed to create main window\n");
		return false;
	}

	// Show window and set focus
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}


int MainWindow::Run() {
	MSG   msg = { 0 };
	bool done = false;

	// Main loop
	while (!done) {

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				done = true;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Process frame
			if (!m_System->Tick()) {
				MessageBox(m_hWnd, L"Frame processing failed", L"Error", MB_OK);
				return 1;
			}
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

		case WM_SIZE:
			if (m_Resizing) {
				// Do nothing. Constantly calling the resize function would be slow.
			}
			else {
				int windowWidth = LOWORD(lParam);
				int windowHeight = HIWORD(lParam);
				m_System->OnResize(windowWidth, windowHeight);
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			m_Resizing = true;
			return 0;

		case WM_EXITSIZEMOVE:
			m_Resizing = false;
			return 0;

		// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}