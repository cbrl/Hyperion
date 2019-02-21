#include "input.h"


//----------------------------------------------------------------------------------
// InputMessageHandler
//----------------------------------------------------------------------------------

// Define the static handler
InputMessageHandler InputMessageHandler::handler;

LRESULT InputMessageHandler::msgProc(gsl::not_null<HWND> window, u32 msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			Mouse::ProcessMessage(msg, wParam, lParam);
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

		default:
			return 0;
	}
}




//----------------------------------------------------------------------------------
// Input
//----------------------------------------------------------------------------------

Input::Input(gsl::not_null<HWND> hWnd) {
	keyboard = std::make_unique<Keyboard>();
	mouse    = std::make_unique<Mouse>();

	mouse->SetWindow(hWnd);
	mouse->SetMode(Mouse::MODE_RELATIVE);
}


void Input::tick() {
	// Read current keyboard state
	keyboard_state = keyboard->GetState();
	keyboard_tracker.Update(keyboard_state);

	// Read current mouse state
	mouse_state = mouse->GetState();
	button_tracker.Update(mouse_state);
}


void Input::reset() {
	keyboard_tracker.Reset();
	button_tracker.Reset();
}


i32_2 Input::getMousePosition() const {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_RELATIVE) {
		return i32_2{0, 0};
	}
	return i32_2{mouse_state.x, mouse_state.y};
}


i32_2 Input::getMouseDelta() const {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
		return i32_2{0, 0};
	}
	return i32_2{mouse_state.x, mouse_state.y};
}
