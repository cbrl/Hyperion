module;

#include "os/windows/windows.h"

export module imgui_message_forwarder;

import window;

// Declare the ImGui msg handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

export class ImGuiMessageForwarder final : public MessageForwarder {
public:
	static ImGuiMessageForwarder forwarder;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ImGuiMessageForwarder() noexcept = default;
	ImGuiMessageForwarder(const ImGuiMessageForwarder& forwarder) noexcept = default;
	ImGuiMessageForwarder(ImGuiMessageForwarder&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ImGuiMessageForwarder() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ImGuiMessageForwarder& operator=(const ImGuiMessageForwarder& forwarder) noexcept = default;
	ImGuiMessageForwarder& operator=(ImGuiMessageForwarder&& forwarder) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void msgProc(gsl::not_null<HWND> window, UINT msg, WPARAM wParam, LPARAM lParam) override {
		ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);
	}
};

// Define the static message forwarder
ImGuiMessageForwarder ImGuiMessageForwarder::forwarder;
