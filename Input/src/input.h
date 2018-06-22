#pragma once

#include "os/windows.h"
#include "datatypes/datatypes.h"
#include <Keyboard.h>
#include <Mouse.h>


class Input final {
public:
	Input(HWND hWnd);
	~Input() = default;

	void tick();
	void reset();

	// Set mouse to use absolute screen coords.
	// Allows the mouse to leave the window.
	void setMouseAbsolute() const {
		mouse->SetMode(Mouse::MODE_ABSOLUTE);
	}

	// Set mouse to use relative window coords. Prevents
	// mouse from leaving the window and hides the cursor.
	void setMouseRelative() const {
		mouse->SetMode(Mouse::MODE_RELATIVE);
	}

	// Toggle the mouse mode.
	void toggleMouseMode() const {
		mouse_state.positionMode == Mouse::MODE_ABSOLUTE
			? mouse->SetMode(Mouse::MODE_RELATIVE)
			: mouse->SetMode(Mouse::MODE_ABSOLUTE);
	}


	// Get mouse movement since last update
	[[nodiscard]]
	int2 getMouseDelta() const;

	// Get the current mouse mode
	[[nodiscard]]
	Mouse::Mode getMouseMode() const {
		return mouse_state.positionMode;
	}


	// Check if key is currently down
	[[nodiscard]]
	bool isKeyDown(Keyboard::Keys key) const {
		return keyboard_state.IsKeyDown(key);
	}

	// Check if key is currently up
	[[nodiscard]]
	bool isKeyUp(Keyboard::Keys key) const {
		return keyboard_state.IsKeyUp(key);
	}


	// Check if key has been pressed. If it has, the function
	// will only return true for the first query
	[[nodiscard]]
	bool isKeyPressed(Keyboard::Keys key) const {
		return keyboard_tracker.IsKeyPressed(key);
	}

	// Check if key has been released. If it has, the function
	// will only return true for the first query
	[[nodiscard]]
	bool isKeyReleased(Keyboard::Keys key) const {
		return keyboard_tracker.IsKeyReleased(key);
	}


private:
	unique_ptr<Keyboard> keyboard;
	unique_ptr<Mouse> mouse;

	Keyboard::KeyboardStateTracker keyboard_tracker;
	Keyboard::State keyboard_state;

	Mouse::ButtonStateTracker button_tracker;
	Mouse::State mouse_state;
};