module;

#include "datatypes/types.h"
#include "directx/directxtk.h"

export module input;

import window;


//----------------------------------------------------------------------------------
// InputMessageHandler
//----------------------------------------------------------------------------------
export class InputMessageHandler final : public MessageHandler {
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
	LRESULT msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) override {
		switch (msg) {
			case WM_ACTIVATEAPP:
				Keyboard::ProcessMessage(msg, wParam, lParam);
				Mouse::ProcessMessage(msg, wParam, lParam);
				return 0;

			// Send keyboard events to keyboard handler
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				Keyboard::ProcessMessage(msg, wParam, lParam);
				return 0;

			// Send mouse events to mouse handler
			case WM_INPUT:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_MOUSEHOVER:
				Mouse::ProcessMessage(msg, wParam, lParam);
				return 0;

			default:
				return 0;
		}
	}
};

// Define the static handler
InputMessageHandler InputMessageHandler::handler;



//----------------------------------------------------------------------------------
// Input
//----------------------------------------------------------------------------------
export class Input final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Input(gsl::not_null<HWND> hWnd) {
		keyboard = std::make_unique<Keyboard>();
		mouse = std::make_unique<Mouse>();

		mouse->SetWindow(hWnd);
		mouse->SetMode(Mouse::MODE_RELATIVE);
	}

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
	void tick() {
		// Read current keyboard state
		keyboard_state = keyboard->GetState();
		keyboard_tracker.Update(keyboard_state);

		// Read current mouse state
		mouse_state = mouse->GetState();
		button_tracker.Update(mouse_state);
	}

	// Reset the input state
	void reset() {
		keyboard_tracker.Reset();
		button_tracker.Reset();
	}


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

	// Get the current mouse mode
	[[nodiscard]]
	Mouse::Mode getMouseMode() const {
		return mouse_state.positionMode;
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
	i32_2 getMousePosition() const {
		// Mouse::MODE_ABSOLUTE - x/y are the position
		// Mouse::MODE_RELATIVE - x/y are the delta

		if (mouse_state.positionMode == Mouse::MODE_RELATIVE) {
			return i32_2{ 0, 0 };
		}
		return i32_2{ mouse_state.x, mouse_state.y };
	}

	// Get the mouse movement since last update. Returns {0, 0} in absolute mode.
	[[nodiscard]]
	i32_2 getMouseDelta() const {
		// Mouse::MODE_ABSOLUTE - x/y are the position
		// Mouse::MODE_RELATIVE - x/y are the delta

		if (mouse_state.positionMode == Mouse::MODE_ABSOLUTE) {
			return i32_2{ 0, 0 };
		}
		return i32_2{ mouse_state.x, mouse_state.y };
	}

	//----------------------------------------------------------------------------------
	// Member Functions - Mouse Pressed
	//----------------------------------------------------------------------------------

	// Check if the left mouse button is pressed. Will only return true once while the button is down.
	[[nodiscard]]
	bool isMouseLeftPressed() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::PRESSED;
	}

	// Check if the right mouse button is pressed. Will only return true once while the button is down.
	[[nodiscard]]
	bool isMouseRightPressed() const {
		return button_tracker.rightButton == Mouse::ButtonStateTracker::PRESSED;
	}

	// Check if mouse button x1 is pressed. Will only return true once while the button is down.
	[[nodiscard]]
	bool isMouseX1Pressed() const {
		return button_tracker.xButton1 == Mouse::ButtonStateTracker::PRESSED;
	}

	// Check if mouse button x2 is pressed. Will only return true once while the button is down.
	[[nodiscard]]
	bool isMouseX2Pressed() const {
		return button_tracker.xButton2 == Mouse::ButtonStateTracker::PRESSED;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Mouse Released
	//----------------------------------------------------------------------------------

	// Check if the left mouse button is released. Will only return true once while the button is up.
	[[nodiscard]]
	bool isMouseLeftReleased() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::RELEASED;
	}

	// Check if the right mouse button is released. Will only return true once while the button is up.
	[[nodiscard]]
	bool isMouseRightReleased() const {
		return button_tracker.rightButton == Mouse::ButtonStateTracker::RELEASED;
	}

	// Check if mouse button x1 is released. Will only return true once while the button is up.
	[[nodiscard]]
	bool isMouseX1Released() const {
		return button_tracker.xButton1 == Mouse::ButtonStateTracker::RELEASED;
	}

	// Check if mouse button x2 is released. Will only return true once while the button is up.
	[[nodiscard]]
	bool isMouseX2Released() const {
		return button_tracker.xButton2 == Mouse::ButtonStateTracker::RELEASED;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Mouse Down
	//----------------------------------------------------------------------------------

	// Check if the left mouse button is down
	[[nodiscard]]
	bool isMouseLeftDown() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::HELD;
	}

	// Check if the right mouse button is down
	[[nodiscard]]
	bool isMouseRightDown() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::HELD;
	}

	// Check if mouse button x1 is down
	[[nodiscard]]
	bool isMouseX1Down() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::HELD;
	}

	// Check if mouse button x2 is down
	[[nodiscard]]
	bool isMouseX2Down() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::HELD;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Mouse Up
	//----------------------------------------------------------------------------------

	// Check if the left mouse button is up
	[[nodiscard]]
	bool isMouseLeftUp() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::UP;
	}

	// Check if the right mouse button is up
	[[nodiscard]]
	bool isMouseRightUp() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::UP;
	}

	// Check if mouse button x1 is up
	[[nodiscard]]
	bool isMouseX1Up() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::UP;
	}

	// Check if mouse button x2 is up
	[[nodiscard]]
	bool isMouseX2Up() const {
		return button_tracker.leftButton == Mouse::ButtonStateTracker::UP;
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

	// Check if key has been pressed. Will only return true once while the key is down.
	[[nodiscard]]
	bool isKeyPressed(Keyboard::Keys key) const {
		return keyboard_tracker.IsKeyPressed(key);
	}

	// Check if key has been released. Will only return true once while the key is up.
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
