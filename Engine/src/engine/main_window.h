#pragma once

#include "os/windows/windows.h"
#include "datatypes/datatypes.h"


class MainWindow {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	MainWindow();
	MainWindow(const MainWindow& window) = delete;
	MainWindow(MainWindow&& window) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~MainWindow() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	MainWindow& operator=(const MainWindow& window) = delete;
	MainWindow& operator=(MainWindow&& window) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	bool initWindow(LPCWSTR name, u32 width, u32 height);

	virtual void run();

	virtual LRESULT msgProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);


protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	HINSTANCE hInstance;
	HWND hWnd;
	LPCWSTR app_name;
	u32 window_width;
	u32 window_height;
};


static MainWindow* handle = nullptr;
