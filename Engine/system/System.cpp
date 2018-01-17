#include "stdafx.h"
#include "System.h"


System::System() {
}


System::~System() {
}


bool System::Init() {
	// Create main window
	if (!InitWindow(L"Engine", WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return false;
	}

	// Initialize Direct3D
	m_Direct3D = make_shared<Direct3D>(m_hWnd, m_WindowWidth, m_WindowHeight, MSAA_STATE, VSYNC_STATE, FULLSCREEN_STATE);
	if (!m_Direct3D->Init()) {
		return false;
	}

	// Initialize DirectInput
	m_Input = make_unique<Input>(m_hWnd);

	// Initialize renderer
	m_Renderer = make_unique<Renderer>(m_hWnd, m_Direct3D);
	if (!m_Renderer->Init()) {
		return false;
	}

	// Initialize scene
	m_Scene = make_unique<Scene>(m_hWnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	if (!m_Scene->Init()) {
		return false;
	}

	// Create Timer
	m_Timer = make_unique<Timer>();

	// Create FPS Counter
	m_FPSCounter = make_unique<FPS>();

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
			if (!Tick()) {
				MessageBox(m_hWnd, L"Frame processing failed", L"Error", MB_OK);
				return 1;
			}

			if (m_Input->IsKeyPressed(Keyboard::Escape)) {
				done = true;
			}
		}
	}

	return 0;
}


bool System::Tick() {
	int   mouseX, mouseY;
	float deltaTime;

	// Update system metrics
	m_Timer->Tick();
	m_FPSCounter->Tick();
	deltaTime = m_Timer->DeltaTime();

	// Process input
	m_Input->Tick();

	// Get mouse position
	m_Input->GetMouseLocation(mouseX, mouseY);

	// Update scene
	m_Scene->Tick(*m_Input, deltaTime);
	m_Scene->UpdateMetrics(m_FPSCounter->GetFPS(), NULL, mouseX, mouseY);

	// Render scene
	if (!m_Renderer->Tick(*m_Scene, deltaTime)) {
		return false;
	}

	return true;
}


LRESULT System::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			Mouse::ProcessMessage(msg, wParam, lParam);
			break;

		// Handle window resize
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

		// Send keyboard events to keyboard handler
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;

		// Send mouse events to mouse handler
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::ProcessMessage(msg, wParam, lParam);
			break;

		// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


void System::OnResize(int windowWidth, int windowHeight) {
	if (m_Direct3D == nullptr) return;
	m_Direct3D->OnResize(windowWidth, windowHeight);
}