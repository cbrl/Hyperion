#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <wrl\client.h>

using Microsoft::WRL::ComPtr;

class Input {
	public:
		Input(HINSTANCE hInstance, HWND hWnd, int windowWidth, int windowHeight);
		~Input();
		
		bool Tick();
		void GetMouseLocation(int &xPos, int &yPos);
		bool IsKeyPressed(int key);


	private:
		void Init(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight);
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();


	private:
		ComPtr<IDirectInput8>       m_DirectInput;
		ComPtr<IDirectInputDevice8> m_Keyboard;
		ComPtr<IDirectInputDevice8> m_Mouse;

		UCHAR        m_KeyboardState[256];
		DIMOUSESTATE m_MouseState;

		int m_WinWidth, m_WinHeight;
		int m_MouseX, m_MouseY;
};
