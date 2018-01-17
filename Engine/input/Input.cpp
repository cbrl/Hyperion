#include "stdafx.h"
#include "input\Input.h"


Input::Input(HWND hWnd) {
	Init(hWnd);
}


Input::~Input() {
}


void Input::Init(HWND hWnd) {
	m_Keyboard = make_unique<Keyboard>();
	m_Mouse    = make_unique<Mouse>();

	m_Mouse->SetWindow(hWnd);
}


void Input::Tick() {
	// Read current keyboard state
	m_KeyboardState = m_Keyboard->GetState();
	m_Tracker.Update(m_KeyboardState);

	// Read current mouse state
	m_MouseState = m_Mouse->GetState();
}


void Input::Reset() {
	m_Tracker.Reset();
}


void Input::GetMouseLocation(int &xPos, int &yPos) {
	xPos = m_MouseState.x;
	yPos = m_MouseState.y;

	return;
}


bool Input::IsKeyPressed(Keyboard::Keys key) {
	if (m_KeyboardState.IsKeyDown(key)) {
		return true;
	}

	return false;
}


bool Input::IsKeyPressedTracker(Keyboard::Keys key) {
	if (m_Tracker.IsKeyPressed(key)) {
		return true;
	}
	return false;
}


bool Input::IsKeyReleasedTracker(Keyboard::Keys key) {
	if (m_Tracker.IsKeyReleased(key)) {
		return true;
	}
	return false;
}