#include "stdafx.h"
#include "System.h"


System::System() {
}


System::~System() {
}


bool System::Init() {
	InitWindow(L"Engine", WINDOW_WIDTH, WINDOW_HEIGHT);

	m_Graphics = make_unique<Graphics>(m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!m_Graphics->Init()) {
		return false;
	}

	return true;
}


int System::Run() {
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
			if (!m_Graphics->Tick()) {
				MessageBox(m_hWnd, L"Frame processing failed", L"Error", MB_OK);
				return 1;
			}
		}
	}

	return 0;
}


LRESULT System::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			m_WindowWidth = LOWORD(lParam);
			m_WindowHeight = HIWORD(lParam);

			if (wParam == SIZE_MAXIMIZED) {
				OnResize(m_WindowWidth, m_WindowHeight);
			}
			else if (wParam == SIZE_RESTORED) {
				if (m_Resizing) {
					// Do nothing. Constantly calling the resize function would be slow.
				}
				else {
					OnResize(m_WindowWidth, m_WindowHeight);
				}
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			m_Resizing = true;
			return 0;

		case WM_EXITSIZEMOVE:
			m_Resizing = false;
			OnResize(m_WindowWidth, m_WindowHeight);
			return 0;

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 240;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 240;
			return 0;

			// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


void System::OnResize(int windowWidth, int windowHeight) {
	m_Graphics->OnResize(windowWidth, windowHeight);
}