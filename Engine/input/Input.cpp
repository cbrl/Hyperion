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


void Input::GetMouseDelta(i32& delta_x, i32& delta_y) const {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
		delta_x = 0;
		delta_y = 0;
	}
	else {
		delta_x = mouse_state.x;
		delta_y = mouse_state.y;
	}
}
