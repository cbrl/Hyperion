#include "rendering_mgr.h"
#include "log/log.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


namespace render {

RenderingMgr::RenderingMgr(gsl::not_null<HWND> window,
                           DisplayConfig display_conf,
                           RenderingConfig rendering_conf) {

	// Store Display/Rendering configuration
	display_config = std::make_unique<DisplayConfig>(std::move(display_conf));
	rendering_config = std::make_unique<RenderingConfig>(std::move(rendering_conf));


	//----------------------------------------------------------------------------------
	// Initialize Direct3D
	//----------------------------------------------------------------------------------
	direct3D = std::make_unique<Direct3D>(*display_config);
	Logger::log(LogLevel::info, "Initialized Direct3D");

	swap_chain = std::make_unique<SwapChain>(window,
	                                         *display_config,
	                                         direct3D->getDevice(),
	                                         direct3D->getDeviceContext());

	Logger::log(LogLevel::info, "Initialized swap chain");


	//----------------------------------------------------------------------------------
	// Create the resource manager
	//----------------------------------------------------------------------------------
	resource_mgr = std::make_unique<ResourceMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


	//----------------------------------------------------------------------------------
	// Create the renderer
	//----------------------------------------------------------------------------------
	renderer = std::make_unique<Renderer>(*display_config,
	                                      *rendering_config,
	                                      direct3D->getDevice(),
	                                      direct3D->getDeviceContext(),
	                                      *swap_chain,
	                                      *resource_mgr);


	//----------------------------------------------------------------------------------
	// Initialize ImGui
	//----------------------------------------------------------------------------------
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(&direct3D->getDevice(), &direct3D->getDeviceContext());
	ImGui::StyleColorsClassic();
}


RenderingMgr::~RenderingMgr() {

	//----------------------------------------------------------------------------------
	// Shutdown ImGui
	//----------------------------------------------------------------------------------
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void RenderingMgr::onResize() const {
	ImGui_ImplDX11_InvalidateDeviceObjects();
	swap_chain->reset();
	renderer->onResize();
	ImGui_ImplDX11_CreateDeviceObjects();
}


void RenderingMgr::render(Scene& scene, std::chrono::duration<f32> delta_time) const {
	renderer->render(scene, delta_time);
}


void RenderingMgr::beginFrame() const {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Clear the render target view with a specified color
	static constexpr f32 color[4] = {0.39f, 0.39f, 0.39f, 1.0f};
	swap_chain->clear(color);
}


void RenderingMgr::endFrame() const {

	// Present the final frame
	swap_chain->present();
}

} //namespace render