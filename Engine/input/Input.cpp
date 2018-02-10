#include "stdafx.h"
#include "input\Input.h"


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
	keyboardState = keyboard->GetState();
	keyboardTracker.Update(keyboardState);

	// Read current mouse state
	lastMouseState = mouseState;
	mouseState = mouse->GetState();
	buttonTracker.Update(mouseState);
}


void Input::Reset() {
	keyboardTracker.Reset();
	buttonTracker.Reset();
}


void Input::GetMouseDelta(int &xPos, int &yPos) {
	// Mouse::MODE_ABSOLUTE - x/y are the position
	// Mouse::MODE_RELATIVE - x/y are the delta

	if (mouseState.positionMode == Mouse::MODE_ABSOLUTE) {
		xPos = mouseState.x - lastMouseState.x;
		yPos = mouseState.y - lastMouseState.y;
	}
	else {
		xPos = mouseState.x;
		yPos = mouseState.y;
	}
}
