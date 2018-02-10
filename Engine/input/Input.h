#pragma once

#include <wrl\client.h>
#include <Keyboard.h>
#include <Mouse.h>

using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Input {
	public:
		Input(HWND hWnd);
		~Input();
		
		void Tick();
		void Reset();

		void SetMouseAbsolute() { mouse->SetMode(Mouse::MODE_ABSOLUTE); }
		void SetMouseRelative() { mouse->SetMode(Mouse::MODE_RELATIVE); }

		void GetMouseDelta(int &xPos, int &yPos);

		bool IsKeyDown(Keyboard::Keys key) { return keyboardState.IsKeyDown(key); }
		bool IsKeyPressed(Keyboard::Keys key) { return keyboardTracker.IsKeyPressed(key); }
		bool IsKeyReleased(Keyboard::Keys key) { return keyboardTracker.IsKeyReleased(key); }


	private:
		void Init(HWND hWnd);


	private:
		unique_ptr<Keyboard> keyboard;
		unique_ptr<Mouse>    mouse;

		Keyboard::KeyboardStateTracker keyboardTracker;
		Keyboard::State                keyboardState;
		Mouse::ButtonStateTracker      buttonTracker;
		Mouse::State                   mouseState;
		Mouse::State                   lastMouseState;
};
