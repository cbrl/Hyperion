#include "stdafx.h"
#include "input\Input.h"


Input::Input(HINSTANCE hInstance, HWND hWnd, int windowWidth, int windowHeight) {
	Init(hInstance, hWnd, windowWidth, windowHeight);
}


Input::~Input() {
	if (m_Keyboard) {
		m_Keyboard->Unacquire();
	}

	if (m_Mouse) {
		m_Mouse->Unacquire();
	}
}


void Input::Init(HINSTANCE hInstance, HWND hWnd, int windowWidth, int windowHeight) {
	HRESULT result;

	m_WinWidth = windowWidth;
	m_WinHeight = windowHeight;

	m_MouseX = 0;
	m_MouseY = 0;


	// Initialze main direct input interface
	HR(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)m_DirectInput.ReleaseAndGetAddressOf(), NULL));

	// Initialize keyboard interface
	HR(m_DirectInput->CreateDevice(GUID_SysKeyboard, m_Keyboard.ReleaseAndGetAddressOf(), NULL));

	// Set data format to predefined keyboard format
	HR(m_Keyboard->SetDataFormat(&c_dfDIKeyboard));

	// Set cooporative level of keyboard to not share with other programs
	HR(m_Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	// Acquire keyboard
	HR(m_Keyboard->Acquire());


	// Initialize mouse interface
	result = m_DirectInput->CreateDevice(GUID_SysMouse, m_Mouse.ReleaseAndGetAddressOf(), NULL);

	// Set data format to predefined mouse format
	HR(m_Mouse->SetDataFormat(&c_dfDIMouse));

	// Set cooporative level of mouse to not share with other programs
	HR(m_Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	// Acquire mouse
	HR(m_Mouse->Acquire());
}


bool Input::Tick() {
	bool result;

	// Read current keyboard state
	result = ReadKeyboard();
	if (!result) return false;

	// Read current mouse state
	result = ReadMouse();
	if (!result) return false;

	// Process changes in input
	ProcessInput();

	return true;
}


bool Input::ReadKeyboard() {
	HRESULT result;

	// Read keyboard
	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			m_Keyboard->Acquire();
		}
		else return false;
	}

	return true;
}


bool Input::ReadMouse() {
	HRESULT result;

	// Read keyboard
	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			m_Mouse->Acquire();
		}
		else return false;
	}

	return true;
}


void Input::ProcessInput() {
	// Update mouse position
	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	// Ensure mouse doesn't leave window space
	if (m_MouseX < 0) m_MouseX = 0;
	if (m_MouseY < 0) m_MouseY = 0;
	if (m_MouseX > m_WinWidth) m_MouseX = m_WinWidth;
	if (m_MouseY > m_WinHeight) m_MouseY = m_WinHeight;

	return;
}


void Input::GetMouseLocation(int &xPos, int &yPos) {
	xPos = m_MouseX;
	yPos = m_MouseY;

	return;
}


bool Input::IsKeyPressed(int key) {
	if (m_KeyboardState[key] & 0x80) {
		return true;
	}

	return false;
}
