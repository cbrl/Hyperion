#pragma once

#include "os/windows/windows.h"


//----------------------------------------------------------------------------------
// MessageForwarder
//----------------------------------------------------------------------------------
class MessageForwarder {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MessageForwarder() noexcept = default;
	MessageForwarder(const MessageForwarder& forwarder) noexcept = default;
	MessageForwarder(MessageForwarder&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~MessageForwarder() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MessageForwarder& operator=(const MessageForwarder& forwarder) noexcept = default;
	MessageForwarder& operator=(MessageForwarder&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	virtual void msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};


//----------------------------------------------------------------------------------
// MessageHandler
//----------------------------------------------------------------------------------
class MessageHandler {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MessageHandler() noexcept = default;
	MessageHandler(const MessageHandler& forwarder) noexcept = default;
	MessageHandler(MessageHandler&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~MessageHandler() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MessageHandler& operator=(const MessageHandler& forwarder) noexcept = default;
	MessageHandler& operator=(MessageHandler&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	virtual LRESULT msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};




//----------------------------------------------------------------------------------
// WindowConfig
//----------------------------------------------------------------------------------
class WindowClass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WindowClass(gsl::not_null<HINSTANCE> instance,
				 std::wstring window_class_name = L"Engine",
				 u32 window_class_style = CS_HREDRAW | CS_VREDRAW);

	WindowClass(const WindowClass&) = delete;
	WindowClass(WindowClass&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~WindowClass();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	WindowClass& operator=(const WindowClass&) = delete;
	WindowClass& operator=(WindowClass&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	gsl::not_null<HINSTANCE> getInstance() const noexcept {
		return gsl::not_null<HINSTANCE>(instance);
	}

	[[nodiscard]]
	const std::wstring& getClassName() const noexcept {
		return class_name;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	HINSTANCE instance;
	std::wstring class_name;
};



//----------------------------------------------------------------------------------
// Window
//----------------------------------------------------------------------------------
class Window final {
public:
	//----------------------------------------------------------------------------------
	// Static Members
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	static LRESULT CALLBACK wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Window(std::shared_ptr<WindowClass> window_class,
	       const std::wstring& title,
	       u32_2 resolution,
	       DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	Window(const Window& window) = delete;
	Window(Window&& window) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Window() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Window& operator=(const Window& window) = delete;
	Window& operator=(Window&& window) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Window
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	WindowClass& getClass() {
		return *win_class;
	}

	[[nodiscard]]
	const WindowClass& getClass() const {
		return *win_class;
	}

	// Get the window's HINSTANCE
	[[nodiscard]]
	gsl::not_null<HINSTANCE> getInstance() const noexcept {
		return win_class->getInstance();
	}

	// Get the window's HWND
	[[nodiscard]]
	HWND getHandle() const noexcept {
		return window;
	}

	// Display the window
	void show(int nCmdShow) const {
		ShowWindow(window, nCmdShow);
	}

	// Get the icon associated with the window's class
	[[nodiscard]]
	HICON getIcon() const {
		return reinterpret_cast<HICON>(GetClassLongPtr(window, GCLP_HICONSM));
	}

	// Set the icon associated with the window's class
	void setIcon(HICON icon) const {
		SetClassLongPtr(window, GCLP_HICONSM, reinterpret_cast<LONG_PTR>(icon));
	}

	// Get the title of the window (as displayed in the title bar)
	[[nodiscard]]
	std::wstring getWindowTitle() const {
		wchar_t title[256];
		GetWindowText(window, title, static_cast<int>(std::size(title)));
		return std::wstring(title);
	}

	// Set the title of the window
	void setWindowTitle(const std::wstring& title) const {
		SetWindowText(window, title.c_str());
	}

	// Get the size of the window (including window decorations)
	[[nodiscard]]
	u32_2 getWindowSize() const {
		RECT wind;
		GetWindowRect(window, &wind);
		return { static_cast<u32>(wind.right),
		         static_cast<u32>(wind.bottom) };
	}

	// Set the size of the window (including window decorations)
	void resizeWindow(u32_2 size) const {
		RECT pos;
		GetWindowRect(window, &pos);
		MoveWindow(window, pos.left, pos.top, size[0], size[1], TRUE);
	}

	// Get the top-left position of the window (including window decorations)
	[[nodiscard]]
	u32_2 getWindowPosition() const {
		RECT pos;
		GetWindowRect(window, &pos);
		return { static_cast<u32>(pos.left),
			     static_cast<u32>(pos.top) };
	}

	// Set the top-left position of the window (including window decorations)
	void setWindowPosition(u32_2 pos) const {
		RECT rect;
		GetWindowRect(window, &rect);
		MoveWindow(window, pos[0], pos[1], rect.right - rect.left, rect.bottom - rect.top, TRUE);
	}

	// Get the size of the client within the window
	[[nodiscard]]
	u32_2 getClientSize() const {
		RECT client;
		GetClientRect(window, &client);
		return { static_cast<u32>(client.right),
		         static_cast<u32>(client.bottom) };
	}

	// Set the size of the client in the window
	void resizeClient(u32_2 size) const {
		RECT pos;
		GetWindowRect(window, &pos);
		RECT client = { 0, 0, static_cast<LONG>(size[0]), static_cast<LONG>(size[1]) };
		AdjustWindowRect(&client, GetWindowLong(window, GWL_STYLE), false);
		MoveWindow(window, pos.left, pos.top, client.right - client.left, client.bottom - client.top, TRUE);
	}

	// Get the top-left position of the client in the window
	[[nodiscard]]
	u32_2 getClientPosition() const {
		RECT  client;
		POINT pos;
		GetClientRect(window, &client);
		pos.x = client.left;
		pos.y = client.top;
		ClientToScreen(window, &pos);
		return { static_cast<u32>(pos.x),
		         static_cast<u32>(pos.y) };
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Forwarders
	//----------------------------------------------------------------------------------
	void addForwarder(gsl::not_null<MessageForwarder*> forwarder) {
		forwarders.push_back(forwarder);
	}

	void removeForwarder(gsl::not_null<MessageForwarder*> forwarder) {
		forwarders.erase(std::remove(std::begin(forwarders), std::end(forwarders),
		                             forwarder),
		                 std::end(forwarders));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Handlers
	//----------------------------------------------------------------------------------
	void addHandler(gsl::not_null<MessageHandler*> handler) {
		handlers.push_back(handler);
	}

	void removeHandler(gsl::not_null<MessageHandler*> handler) {
		handlers.erase(std::remove(std::begin(handlers), std::end(handlers),
		                           handler),
		               std::end(handlers));
	}


private:
	void init(const std::wstring& title, u32_2 resolution, DWORD style);

	[[nodiscard]]
	LRESULT msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::shared_ptr<WindowClass> win_class;
	HWND window;

	std::vector<gsl::not_null<MessageForwarder*>> forwarders;
	std::vector<gsl::not_null<MessageHandler*>> handlers;
};