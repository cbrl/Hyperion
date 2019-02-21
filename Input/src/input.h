#pragma once

#include "datatypes/datatypes.h"
#include "directx/directxtk.h"
#include "os/windows/window.h"


//----------------------------------------------------------------------------------
// InputMessageHandler
//----------------------------------------------------------------------------------
class InputMessageHandler final : public MessageHandler {
public:
	static InputMessageHandler handler;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	InputMessageHandler() noexcept = default;
	InputMessageHandler(const InputMessageHandler& handler) noexcept = default;
	InputMessageHandler(InputMessageHandler&& handler) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~InputMessageHandler() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	InputMessageHandler& operator=(const InputMessageHandler& handler) noexcept = default;
	InputMessageHandler& operator=(InputMessageHandler&& handler) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	// Updates DirectXTK mouse and keyboard state
	[[nodiscard]]
	LRESULT msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) override;
};




//----------------------------------------------------------------------------------
// Input
//----------------------------------------------------------------------------------
class Input final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Input(gsl::not_null<HWND> hWnd);
	Input(const Input& input) = delete;
	Input(Input&& input) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Input() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Input& operator=(const Input& input) = delete;
	Input& operator=(Input&& input) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Update State
	//----------------------------------------------------------------------------------

	// Update the input state
	void tick();

	// Reset the input state
	void reset();


	//----------------------------------------------------------------------------------
	// Member Functions - Mouse
	//----------------------------------------------------------------------------------

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
			? setMouseRelative()
			: setMouseAbsolute();
	}

	// Set the mouse visibility (absolute mode only)
	void setMouseVisible(bool state) {
		mouse->SetVisible(state);
	}

	// Toggle the mouse visibility (absolute mode only)
	void toggleMouseVisible() {
		mouse->IsVisible() ? mouse->SetVisible(false) : mouse->SetVisible(true);
	}

	// Get the mouse's current position. Use getMouseDelta() when processing movement.
	// Returns {0, 0} if the mouse is in Relative mode.
	[[nodiscard]]
	i32_2 getMousePosition() const;

	// Get the mouse movement since last update. Returns {0, 0} in absolute mode.
	[[nodiscard]]
	i32_2 getMouseDelta() const;

	// Get the current mouse mode
	[[nodiscard]]
	Mouse::Mode getMouseMode() const {
		return mouse_state.positionMode;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Keyboard
	//----------------------------------------------------------------------------------

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


	// Check if key has been pressed. Will only return true for the first frame.
	[[nodiscard]]
	bool isKeyPressed(Keyboard::Keys key) const {
		return keyboard_tracker.IsKeyPressed(key);
	}

	// Check if key has been released. Will only return true for the first frame.
	[[nodiscard]]
	bool isKeyReleased(Keyboard::Keys key) const {
		return keyboard_tracker.IsKeyReleased(key);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;

	Keyboard::KeyboardStateTracker keyboard_tracker;
	Keyboard::State keyboard_state;

	Mouse::ButtonStateTracker button_tracker;
	Mouse::State mouse_state;
};
