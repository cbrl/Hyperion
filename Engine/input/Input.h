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
		void GetMouseDelta(int &xPos, int &yPos);
		bool IsKeyDown(Keyboard::Keys key);
		bool IsKeyPressed(Keyboard::Keys key);
		bool IsKeyReleased(Keyboard::Keys key);


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
