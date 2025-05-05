#pragma once

#include "os/windows/windows.h"

// Declare the ImGui msg handler. imgui_impl_win32.h does not do this in order to avoid including
// Windows.h. However, this declaration can't be done inside a module, so this header exists solely
// to declare this function.

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
