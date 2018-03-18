#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "util\datatypes\datatypes.h"


class Input {
	public:
		Input(HWND hWnd);
		~Input();
		
		void Tick();
		void Reset();

		void SetMouseAbsolute() { mouse->SetMode(Mouse::MODE_ABSOLUTE); }
		void SetMouseRelative() { mouse->SetMode(Mouse::MODE_RELATIVE); }

		void GetMouseDelta(i32& xPos, i32& yPos);

		bool IsKeyDown(Keyboard::Keys key) { return keyboard_state.IsKeyDown(key); }
		bool IsKeyPressed(Keyboard::Keys key) { return keyboard_tracker.IsKeyPressed(key); }
		bool IsKeyReleased(Keyboard::Keys key) { return keyboard_tracker.IsKeyReleased(key); }


	private:
		void Init(HWND hWnd);


	private:
		unique_ptr<Keyboard> keyboard;
		unique_ptr<Mouse>    mouse;

		Keyboard::KeyboardStateTracker keyboard_tracker;
		Keyboard::State                keyboard_state;
		Mouse::ButtonStateTracker      button_tracker;
		Mouse::State                   mouse_state;
		Mouse::State                   last_mouse_state;
};
