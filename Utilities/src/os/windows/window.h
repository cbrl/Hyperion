#pragma once

#include "os/windows/windows.h"
#include <algorithm>


//----------------------------------------------------------------------------------
// MessageForwarder
//----------------------------------------------------------------------------------
class MessageForwarder {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MessageForwarder() = default;
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
	virtual void msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};


//----------------------------------------------------------------------------------
// MessageHandler
//----------------------------------------------------------------------------------
class MessageHandler {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MessageHandler() = default;
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
	virtual LRESULT msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};




//----------------------------------------------------------------------------------
// WindowConfig
//----------------------------------------------------------------------------------
class WindowConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WindowConfig(HINSTANCE instance,
				 std::wstring window_class_name,
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
	HINSTANCE getInstance() const noexcept {
		return instance;
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
	       u32 width,
	       u32 height,
	       DWORD style = WS_OVERLAPPEDWINDOW);

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
	HINSTANCE getInstance() const noexcept {
		return config->getInstance();
	}

	[[nodiscard]]
	HWND getWindow() const noexcept {
		return window;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Forwarders
	//----------------------------------------------------------------------------------
	void addForwarder(MessageForwarder* forwarder) {
		forwarders.push_back(forwarder);
	}

	void removeForwarder(MessageForwarder* forwarder) {
		forwarders.erase(std::remove(std::begin(forwarders), std::end(forwarders),
		                             forwarder),
		                 std::end(forwarders));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Handlers
	//----------------------------------------------------------------------------------
	void addHandler(MessageHandler* handler) {
		handlers.push_back(handler);
	}

	void removeHandler(MessageHandler* handler) {
		handlers.erase(std::remove(std::begin(handlers), std::end(handlers),
		                           handler),
		               std::end(handlers));
	}


private:
	void init(const std::wstring& title, u32 width, u32 height, DWORD style);

	[[nodiscard]]
	LRESULT msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::shared_ptr<WindowConfig> config;
	HWND window;

	std::vector<MessageForwarder*> forwarders;
	std::vector<MessageHandler*> handlers;
};