#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include <wrl\client.h>

using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Input {
	public:
		Input(HWND hWnd);
		~Input();
		
		void Tick();
		void Reset();
		void GetMouseLocation(int &xPos, int &yPos);
		bool IsKeyPressed(Keyboard::Keys key);
		bool IsKeyPressedTracker(Keyboard::Keys key);
		bool IsKeyReleasedTracker(Keyboard::Keys key);


	private:
		void Init(HWND hWnd);


	private:
		unique_ptr<Keyboard> m_Keyboard;
		unique_ptr<Mouse>    m_Mouse;

		Keyboard::KeyboardStateTracker m_Tracker;
		Keyboard::State                m_KeyboardState;
		Mouse::State                   m_MouseState;
};
