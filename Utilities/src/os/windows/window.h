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
class WindowConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WindowConfig(gsl::not_null<HINSTANCE> instance,
				 std::wstring window_class_name = L"Engine",
				 u32 window_class_style = CS_HREDRAW | CS_VREDRAW);

	WindowConfig(const WindowConfig& config) = delete;
	WindowConfig(WindowConfig&& config) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~WindowConfig();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	WindowConfig& operator=(const WindowConfig& config) = delete;
	WindowConfig& operator=(WindowConfig&& config) noexcept = default;


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
	Window(std::shared_ptr<WindowConfig> window_config,
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
	// Member Functions - Misc
	//----------------------------------------------------------------------------------
	void show(int nCmdShow) const noexcept {
		ShowWindow(window, nCmdShow);
	}

	[[nodiscard]]
	gsl::not_null<HINSTANCE> getInstance() const noexcept {
		return config->getInstance();
	}

	[[nodiscard]]
	HWND getHandle() const noexcept {
		return window;
	}

	[[nodiscard]]
	std::wstring getWindowTitle() const noexcept {
		wchar_t title[256];
		GetWindowText(window, title, static_cast<int>(std::size(title)));
		return std::wstring(title);
	}

	void setWindowTitle(const std::wstring& title) noexcept {
		SetWindowText(window, title.c_str());
	}

	[[nodiscard]]
	u32_2 getClientSize() const noexcept {
		RECT client;
		GetClientRect(window, &client);
		return { static_cast<u32>(client.right),
		         static_cast<u32>(client.bottom) };
	}

	void resizeWindow(u32_2 size) const noexcept {
		RECT pos;
		GetWindowRect(window, &pos);
		RECT client = { 0, 0, static_cast<LONG>(size[0]), static_cast<LONG>(size[1]) };
		AdjustWindowRect(&client, GetWindowLong(window, GWL_STYLE), false);
		MoveWindow(window, pos.left, pos.top, client.right - client.left, client.bottom - client.top, TRUE);
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
	std::shared_ptr<WindowConfig> config;
	HWND window;

	std::vector<gsl::not_null<MessageForwarder*>> forwarders;
	std::vector<gsl::not_null<MessageHandler*>> handlers;
};