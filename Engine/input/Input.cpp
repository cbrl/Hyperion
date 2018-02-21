#include "stdafx.h"
#include "input.h"


Input::Input(HWND hWnd) {
	Init(hWnd);
}


Input::~Input() {
}


void Input::Init(HWND hWnd) {
	keyboard = make_unique<Keyboard>();
	mouse    = make_unique<Mouse>();

	mouse->SetWindow(hWnd);
	mouse->SetMode(Mouse::MODE_RELATIVE);
}


void Input::Tick() {
	// Read current keyboard state
	keyboard_state = keyboard->GetState();
	keyboard_tracker.Update(keyboard_state);

	// Read current mouse state
	last_mouse_state = mouse_state;
	mouse_state = mouse->GetState();
	button_tracker.Update(mouse_state);
}


void Input::Reset() {
	keyboard_tracker.Reset();
	button_tracker.Reset();
}


void Input::GetMouseDelta(int &xPos, int &yPos) {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
		xPos = mouse_state.x - last_mouse_state.x;
		yPos = mouse_state.y - last_mouse_state.y;
	}
	else {
		xPos = mouse_state.x;
		yPos = mouse_state.y;
	}
}
