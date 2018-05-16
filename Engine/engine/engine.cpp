#include "stdafx.h"
#include "engine.h"
#include "imgui\imgui.h"


// Declare the ImGui msg handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Engine::~Engine() {
	// Explicity delete the scene and entity component system before
	// the rendering manager. This avoids D3D reporting live resources
	// that are going to be deleted right after the report.
	scene.reset();
	ecs_engine.reset();
}


bool Engine::Init() {

	// Set width/height variables. Later on this can be read from a config file.
	window_width  = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;


	// Create main window
	if (!this->InitWindow(L"Engine", window_width, window_height)) {
		return false;
	}
	FILE_LOG(logINFO) << "Initialized main window";


	// Initialize the Entity Component System
	ecs_engine = make_unique<ECS>();
	ecs_engine->AddSystem<CameraSystem>();
	ecs_engine->AddSystem<TransformSystem>();
	ecs_engine->AddSystem<ModelSystem>();


	// Initialize system monitor
	system_monitor = make_unique<SystemMonitor>();


	// Create input handler
	input = make_unique<Input>(hWnd);


	// Create Timer
	timer = make_unique<Timer>();


	// Create FPS Counter
	fps_counter = make_unique<FPS>();


	// Initialize rendering manager
	rendering_mgr = make_unique<RenderingMgr>(*this, FULLSCREEN_STATE, VSYNC_STATE, MSAA_STATE);
	ecs_engine->AddSystem<Renderer>(rendering_mgr->GetDevice(), rendering_mgr->GetDeviceContext());


	// Initialize scene
	scene = make_unique<TestScene>(*this);
	FILE_LOG(logINFO) << "Loaded scene";


	return true;
}


void Engine::Run() {
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

			// Process input
			ProcessInput();

			// Quit if escape is pressed
			if (input->IsKeyDown(Keyboard::Escape)) {
				done = true;
			}
		}
	}
}


void Engine::Tick() {	

	// Update system metrics
	system_monitor->Tick();
	timer->Tick();
	fps_counter->Tick();

	// Update the input state
	input->Tick();

	// Update the scene
	scene->Tick(*this);



	// Begin a new frame
	rendering_mgr->BeginFrame();

	// Update the active systems in the ecs engine
	ecs_engine->Update(*this);

	// Present the frame
	rendering_mgr->EndFrame();
}


void Engine::ProcessInput() {

	// Toggle mouse mode on F1 press
	if (input->IsKeyPressed(Keyboard::F1)) {
		if (input->GetMouseMode() == Mouse::MODE_ABSOLUTE) {
			input->SetMouseRelative();
		}
		else {
			input->SetMouseAbsolute();
		}
	}
}


LRESULT Engine::MsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {

	// Send events to ImGui handler if the mouse mode is set to absolute
	if (input && (input->GetMouseMode() == Mouse::MODE_ABSOLUTE)) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
			return 0;
		}
	}

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
				// Do nothing if resizing. Constantly calling the resize function would be slow.
				if (!resizing) {
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


void Engine::OnResize(u32 window_width, u32 window_height) {

	if (!rendering_mgr) return;
	rendering_mgr->ResizeBuffers(window_width, window_height);
	
	if (scene) {
		if (const auto cam = ecs_engine->GetComponent<PerspectiveCamera>(scene->GetCamera())) {
			cam->ResizeViewport(rendering_mgr->GetDeviceContext(), window_width, window_height);
		}
		else if (const auto cam = ecs_engine->GetComponent<OrthographicCamera>(scene->GetCamera())) {
			cam->ResizeViewport(rendering_mgr->GetDeviceContext(), window_width, window_height);
		}
	}

	FILE_LOG(logINFO) << "Viewport resized to " << window_width << "x" << window_height;
}