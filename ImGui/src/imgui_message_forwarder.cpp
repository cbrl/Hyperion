#include "imgui_message_forwarder.h"

// Declare the ImGui msg handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Define the static message forwarder
ImGuiMessageForwarder ImGuiMessageForwarder::forwarder;

void ImGuiMessageForwarder::msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);
}