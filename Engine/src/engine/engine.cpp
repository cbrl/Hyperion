#include "stdafx.h"
#include "engine.h"
#include "systems/systems.h"


// Declare the ImGui msg handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Engine::~Engine() {

	Logger::log(LogLevel::info, "<==========================END==========================>\n");

	// Explicity delete the scene and entity component system before
	// the rendering manager. This prevents D3D from reporting live
	// resources that are going to be deleted right after the report.
	scene.reset();
	ecs_engine.reset();
}


bool Engine::init() {
	Logger::log(LogLevel::info, "<=========================START=========================>");

	// Create the display configuration
	DisplayConfig config(AAType::none, false, false);

	// Create main window
	if (!this->initWindow(L"Engine", config.getDisplayDesc().Width, config.getDisplayDesc().Height)) {
		return false;
	}
	Logger::log(LogLevel::info, "Initialized main window");


	// Create the Entity Component System
	ecs_engine = make_unique<ECS>();


	// Create the system monitor
	system_monitor = make_unique<SystemMonitor>();


	// Create the input handler
	input = make_unique<Input>(hWnd);


	// Create the timer
	timer = make_unique<HighResTimer>();


	// Create FPS Counter
	fps_counter = make_unique<FPS>();


	// Initialize the rendering manager and rendering system
	rendering_mgr = make_unique<RenderingMgr>(hWnd, config);


	// Add systems to the ECS
	addSystems();


	return true;
}


void Engine::addSystems() const {

	// Transform system: updates transforms when they're modified
	ecs_engine->addSystem<TransformSystem>();

	// Camera system: updates camera buffers
	ecs_engine->addSystem<CameraSystem>();

	// Model system: updates model buffers
	ecs_engine->addSystem<ModelSystem>();

	// Camera motor system: moves an entity with a camera and camera movement component
	ecs_engine->addSystem<CameraMotorSystem>();

	// Mouse rotation system: uses mouse input to rotate an entity's transform
	ecs_engine->addSystem<MouseRotationSystem>();

	ecs_engine->addSystem<AxisRotationSystem>();
	ecs_engine->addSystem<AxisOrbitSystem>();
}


void Engine::loadScene(unique_ptr<Scene>&& new_scene) {
	
	if (scene) {
		scene->unload(*this);
		Logger::log(LogLevel::info, "Unloaded scene: {}", scene->getName());
	}

	scene = std::move(new_scene);

	if (scene) {
		Logger::log(LogLevel::info, "Loading scene: {}", scene->getName());
		scene->load(*this);
		Logger::log(LogLevel::info, "Finished loading scene: {}", scene->getName());
		timer->reset();
	}
}


void Engine::run() {
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
			tick();

			// Process input
			processInput();

			// Quit if escape is pressed
			if (input->isKeyDown(Keyboard::Escape)) {
				done = true;
			}
		}
	}
}


void Engine::tick() const {

	// Update the active systems in the ecs engine
	ecs_engine->update(*this);

	// Update system metrics
	system_monitor->tick();
	timer->tick();
	fps_counter->tick();

	// Update the input state
	input->tick();

	// Update the scene
	scene->tick(*this);

	// Render the scene
	rendering_mgr->render(*this);
}


void Engine::processInput() const {

	// Toggle mouse mode on F1 press
	if (input->isKeyPressed(Keyboard::F1)) {
		if (input->getMouseMode() == Mouse::MODE_ABSOLUTE) {
			input->setMouseRelative();
		}
		else {
			input->setMouseAbsolute();
		}
	}
}


LRESULT Engine::msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {

	// Send events to ImGui handler if the mouse mode is set to absolute
	if (input && (input->getMouseMode() == Mouse::MODE_ABSOLUTE)) {
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	// Send events to the Input message handler
	inputMsgProc(hWnd, msg, wParam, lParam);

	switch (msg) {
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		// Handle window resize
		case WM_SIZE:
		{
			window_width = LOWORD(lParam);
			window_height = HIWORD(lParam);

			if (wParam == SIZE_MAXIMIZED) {
				onResize(window_width, window_height);
			}
			else if (wParam == SIZE_RESTORED) {
				// Do nothing if resizing. Constantly calling the resize function would be slow.
				if (!resizing) {
					onResize(window_width, window_height);
				}
			}

			return 0;
		}

		case WM_ENTERSIZEMOVE:
			resizing = true;
			return 0;

		case WM_EXITSIZEMOVE:
			resizing = false;
			onResize(window_width, window_height);
			return 0;

		case WM_GETMINMAXINFO:
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 240;
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 240;
			return 0;

		// Send other messages to default message handler
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


void Engine::onResize(u32 window_width, u32 window_height) {

	if (!rendering_mgr) return;
	rendering_mgr->resizeBuffers(window_width, window_height);

	if (ecs_engine) {
		ecs_engine->forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
			camera.resizeViewport(window_width, window_height);
		});

		ecs_engine->forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
			camera.resizeViewport(window_width, window_height);
		});
	}

	Logger::log(LogLevel::info, "Viewport resized to {}x{}", window_width, window_height);
}
