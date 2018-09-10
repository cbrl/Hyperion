#include "window.h"

//----------------------------------------------------------------------------------
// Global Functions
//----------------------------------------------------------------------------------

Window* GetCaller(gsl::not_null<HWND> window,
                  UINT msg,
                  WPARAM wParam,
                  LPARAM lParam) {

	if (msg == WM_NCCREATE) {
		const auto params = reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;
		const auto caller = reinterpret_cast<Window*>(params);

		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(caller));

		return caller;
	}

	return reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
}




//----------------------------------------------------------------------------------
// WindowConfig
//----------------------------------------------------------------------------------

WindowConfig::WindowConfig(gsl::not_null<HINSTANCE> instance,
                           std::wstring window_class_name,
						   u32 window_class_style)
	: instance(instance)
	, class_name(std::move(window_class_name)) {

	// Window class definition
	WNDCLASSEX wc = {};
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = window_class_style;
	wc.lpfnWndProc   = Window::wndProc;
	wc.hInstance     = instance;
	wc.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm       = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = class_name.c_str();

	// Register the window class
	ThrowIfFailed(RegisterClassEx(&wc) != 0, "Failed to register window class");
}


WindowConfig::~WindowConfig() {
	UnregisterClass(class_name.c_str(), instance);
}




//----------------------------------------------------------------------------------
// Window
//----------------------------------------------------------------------------------

Window::Window(std::shared_ptr<WindowConfig> window_config,
               const std::wstring& title,
			   vec2_u32 resolution,
               DWORD style)
	: config(std::move(window_config))
	, window(nullptr) {

	assert(config);
	init(title, resolution, style);
}


void Window::init(const std::wstring& title,
				  vec2_u32 resolution,
                  DWORD style) {

	// Create a rect for the adjusted window size
	RECT window_rect = { 0,
	                     0,
	                     static_cast<LONG>(resolution.x),
	                     static_cast<LONG>(resolution.y) };

	// Calculate the total window size with decorations included
	AdjustWindowRect(&window_rect, style, false);

	// Proper window width/height
	const u32 window_width  = window_rect.right - window_rect.left;
	const u32 window_height = window_rect.bottom - window_rect.top;

	// Centered window coords
	const u32 x_pos = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
	const u32 y_pos = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;

	// Create window and store window handle
	window = CreateWindow(config->getClassName().c_str(),
						  title.c_str(),
						  style,
						  x_pos,
						  y_pos,
						  window_width,
						  window_height,
						  nullptr,
						  nullptr,
						  config->getInstance(),
						  this);
	
	ThrowIfFailed(window != nullptr, "Failed to create window");
}


LRESULT CALLBACK Window::wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {

	const auto hwnd = gsl::make_not_null(window);
	auto* caller = GetCaller(hwnd, msg, wParam, lParam);

	if (caller) {
		const auto result = caller->msgProc(hwnd, msg, wParam, lParam);

		if (result) {
			return result;
		}
	}

	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		default:
			return DefWindowProc(window, msg, wParam, lParam);
	}
}


LRESULT Window::msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) {

	for (auto forwarder : forwarders) {
		forwarder->msgProc(window, msg, wParam, lParam);
	}

	for (auto handler : handlers) {
		const auto result = handler->msgProc(window, msg, wParam, lParam);

		if (result)
			return result;
	}

	return 0;
}
