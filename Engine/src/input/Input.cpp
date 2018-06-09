#include "stdafx.h"
#include "input.h"


Input::Input(HWND hWnd) {
	keyboard = make_unique<Keyboard>();
	mouse = make_unique<Mouse>();

	mouse->SetWindow(hWnd);
	mouse->SetMode(Mouse::MODE_RELATIVE);
}


void Input::Tick() {
	// Read current keyboard state
	keyboard_state = keyboard->GetState();
	keyboard_tracker.Update(keyboard_state);

	// Read current mouse state
	mouse_state = mouse->GetState();
	button_tracker.Update(mouse_state);
}


void Input::Reset() {
	keyboard_tracker.Reset();
	button_tracker.Reset();
}


int2 Input::GetMouseDelta() const {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
		return int2(0, 0);
	}
	return int2(mouse_state.x, mouse_state.y);
}
