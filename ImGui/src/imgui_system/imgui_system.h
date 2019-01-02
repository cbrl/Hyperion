#pragma once

#include "system/system.h"
#include "directx/d3d11.h"

#include "scene/events/core_events.h"


class ImGuiSystem final : public System<ImGuiSystem>, public EventListener {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ImGuiSystem(HWND window,
	            ID3D11Device& device,
	            ID3D11DeviceContext& device_context);

	ImGuiSystem(const ImGuiSystem&) = delete;
	ImGuiSystem(ImGuiSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ImGuiSystem();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ImGuiSystem& operator=(const ImGuiSystem&) = delete;
	ImGuiSystem& operator=(ImGuiSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Events
	//----------------------------------------------------------------------------------
	void registerCallbacks() override final;
	void onResize(const WindowResizeEvent* event);


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------
	void preUpdate(Engine& engine) override final;
	void postUpdate(Engine& engine) override final;
};