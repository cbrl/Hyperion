#include "stdafx.h"
#include "system.h"


System* System::system_ptr = nullptr;


System::System() {
	system_ptr = this;
}


System::~System() {
	system_ptr = nullptr;
}


bool System::Init() {
	// Set width/height variables. Later on this can be read from a config file.
	window_width  = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;

	// Create main window
	if (!InitWindow(L"Engine", window_width, window_height)) {
		return false;
	}

	// Initialize rendering manager
	rendering_mgr = make_unique<RenderingMgr>();
	rendering_mgr->Init(window_width, window_height, FULLSCREEN_STATE, VSYNC_STATE, MSAA_STATE);

	// Create input handler
	input = make_unique<Input>(hWnd);

	// Create Timer
	timer = make_unique<Timer>();

	// Create FPS Counter
	fps_counter = make_unique<FPS>();

	// Initialize scene
	scene = make_unique<Scene>(rendering_mgr->GetDevice(), rendering_mgr->GetDeviceContext(), *rendering_mgr->GetResourceMgr());

	return true;
}


void System::Run() {
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
			Tick();

			if (input->IsKeyDown(Keyboard::Escape)) {
				done = true;
			}
		}
	}
}


void System::Tick() {
	i32   mouseX, mouseY;
	float deltaTime;

	// Update system metrics
	timer->Tick();
	fps_counter->Tick();
	deltaTime = timer->DeltaTime();

	// Process input
	input->Tick();

	// Get mouse position
	input->GetMouseDelta(mouseX, mouseY);

	// Update scene
	scene->Tick(*input, deltaTime);
	scene->UpdateMetrics(fps_counter->GetFPS(), NULL, mouseX, mouseY);

	// Render scene
	rendering_mgr->Render(*scene);
}


LRESULT System::MsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			Mouse::ProcessMessage(msg, wParam, lParam);
			return 0;

		// Handle window resize
		case WM_SIZE:
			window_width = LOWORD(lParam);
			window_height = HIWORD(lParam);

			if (wParam == SIZE_MAXIMIZED) {
				OnResize(window_width, window_height);
			}
			else if (wParam == SIZE_RESTORED) {
				if (resizing) {
					// Do nothing. Constantly calling the resize function would be slow.
				}
				else {
					OnResize(window_width, window_height);
				}
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			resizing = true;
			return 0;

		case WM_EXITSIZEMOVE:
			resizing = false;
			OnResize(window_width, window_height);
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
			return 0;

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
			return 0;

		// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


void System::OnResize(u32 window_width, u32 window_height) const {
	if (!rendering_mgr) return;
	rendering_mgr->OnResize(window_width, window_height);
	
	if (!scene) return;
	scene->camera->OnResize(window_width, window_height);
}