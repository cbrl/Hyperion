#pragma once

#include "os/windows/window.h"


class ImGuiMessageForwarder final : public MessageForwarder {
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
	void msgProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) override;
};