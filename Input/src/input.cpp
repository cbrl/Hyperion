#include "input.h"


void inputMsgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			Mouse::ProcessMessage(msg, wParam, lParam);
			return;

		// Send keyboard events to keyboard handler
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			return;

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
			return;

		default:
			return;
	}
}


Input::Input(HWND hWnd) {
	keyboard = make_unique<Keyboard>();
	mouse    = make_unique<Mouse>();

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


[[nodiscard]]
vec2_i32 Input::getMouseDelta() const {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
		return vec2_i32(0, 0);
	}
	return vec2_i32(mouse_state.x, mouse_state.y);
}
