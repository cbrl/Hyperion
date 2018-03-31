#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "util\datatypes\datatypes.h"


class Input final {
	public:
		Input(HWND hWnd);
		~Input() = default;
		
		void Tick();
		void Reset();

		// Set mouse to use absolute screen coords. Allows
		// the mouse to leave the window.
		void SetMouseAbsolute() { mouse->SetMode(Mouse::MODE_ABSOLUTE); }

		// Set mouse to use relative window coords. Prevents
		// mouse from leaving the window and hides the cursor.
		void SetMouseRelative() { mouse->SetMode(Mouse::MODE_RELATIVE); }

		// Toggle the mouse mode.
		void ToggleMouseMode() {
			mouse_state.positionMode == Mouse::MODE_ABSOLUTE ? mouse->SetMode(Mouse::MODE_RELATIVE)
				                                             : mouse->SetMode(Mouse::MODE_ABSOLUTE);
		}

		// Get mouse movement since last update
		void GetMouseDelta(i32& delta_x, i32& delta_y) const;


		// Check if key is currently down
		bool IsKeyDown(Keyboard::Keys key) const { return keyboard_state.IsKeyDown(key); }
		// Check if key is currently up
		bool IsKeyUp(Keyboard::Keys key) const { return keyboard_state.IsKeyUp(key); }


		// Check if key has been pressed. If it has, the function
		// will only return true for the first query
		bool IsKeyPressed(Keyboard::Keys key) const { return keyboard_tracker.IsKeyPressed(key); }

		// Check if key has been released. If it has, the function
		// will only return true for the first query
		bool IsKeyReleased(Keyboard::Keys key) const { return keyboard_tracker.IsKeyReleased(key); }


	private:
		unique_ptr<Keyboard> keyboard;
		unique_ptr<Mouse>    mouse;

		Keyboard::KeyboardStateTracker keyboard_tracker;
		Keyboard::State                keyboard_state;

		Mouse::ButtonStateTracker button_tracker;
		Mouse::State              mouse_state;
};
