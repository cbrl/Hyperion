#include "imgui_system.h"
#include "engine/engine.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


ImGuiSystem::ImGuiSystem(HWND window,
                         ID3D11Device& device,
                         ID3D11DeviceContext& device_context) {

	//----------------------------------------------------------------------------------
	// Initialize ImGui
	//----------------------------------------------------------------------------------
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(&device, &device_context);
	ImGui::StyleColorsClassic();
}


ImGuiSystem::~ImGuiSystem() {

	//----------------------------------------------------------------------------------
	// Shutdown ImGui
	//----------------------------------------------------------------------------------
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void ImGuiSystem::registerCallbacks() {
	registerEventCallback(&ImGuiSystem::onResize);
}


void ImGuiSystem::onResize(const WindowResizeEvent* event) {
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}


void ImGuiSystem::preUpdate(Engine& engine) {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}


void ImGuiSystem::postUpdate(Engine& engine) {

	Pipeline::OM::bindRTVAndDSV(engine.getRenderingMgr().getDeviceContext(), engine.getRenderingMgr().getSwapChain().getRTV(), nullptr);

	// Render the ImGui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}