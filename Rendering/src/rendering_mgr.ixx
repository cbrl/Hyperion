module;

#include <memory>
#include <utility>

#include "datatypes/pointer_types.h"
#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"
#include "log/log.h"

#include "directx/d3d11.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

export module rendering.rendering_mgr;

import rendering.direct3d;
import rendering.display_config;
import rendering.renderer;
import rendering.rendering_config;
import rendering.resource_mgr;
import rendering.scene;
import rendering.swapchain;


namespace render {

export class RenderingMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	RenderingMgr(gsl::not_null<HWND> window,
	             DisplayConfig display_conf,
	             RenderingConfig rendering_conf) {
		// Store Display/Rendering configuration
		display_config = std::make_unique<DisplayConfig>(std::move(display_conf));
		rendering_config = std::make_unique<RenderingConfig>(std::move(rendering_conf));


		// Initialize Direct3D
		direct3D = std::make_unique<Direct3D>(*display_config);
		Logger::log(LogLevel::info, "Initialized Direct3D");

		swap_chain = std::make_unique<SwapChain>(window,
												 *display_config,
												 direct3D->getDevice(),
												 direct3D->getDeviceContext());

		Logger::log(LogLevel::info, "Initialized swap chain");


		// Create the resource manager
		resource_mgr = std::make_unique<ResourceMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


		// Create the renderer
		renderer = std::make_unique<Renderer>(*display_config,
											  *rendering_config,
											  direct3D->getDevice(),
											  direct3D->getDeviceContext(),
											  *swap_chain,
											  *resource_mgr);


		// Initialize ImGui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(&direct3D->getDevice(), &direct3D->getDeviceContext());
		ImGui::StyleColorsClassic();
	}

	RenderingMgr(const RenderingMgr& mgr) = delete;
	RenderingMgr(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~RenderingMgr() {
		// Shutdown ImGui
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	RenderingMgr& operator=(const RenderingMgr& mgr) = delete;
	RenderingMgr& operator=(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Resizing
	//----------------------------------------------------------------------------------

	// Resize the buffers
	void onResize() const {
		ImGui_ImplDX11_InvalidateDeviceObjects();
		swap_chain->reset();
		renderer->onResize();
		ImGui_ImplDX11_CreateDeviceObjects();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rendering
	//----------------------------------------------------------------------------------

	// Start a new frame
	void beginFrame() const {
		// Start a new ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Clear the render target view with a specified color
		static constexpr f32 color[4] = { 0.39f, 0.39f, 0.39f, 1.0f };
		swap_chain->clear(color);
	}

	// End the current frame
	void endFrame() const {
		// Present the final frame
		swap_chain->present();
	}

	// Render the current scene
	void render(Scene& scene, std::chrono::duration<f32> delta_time) const {
		renderer->render(scene, delta_time);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Getters
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DisplayConfig& getDisplayConfig() {
		return *display_config;
	}

	[[nodiscard]]
	const DisplayConfig& getDisplayConfig() const {
		return *display_config;
	}

	[[nodiscard]]
	RenderingConfig& getRenderingConfig() {
		return *rendering_config;
	}

	[[nodiscard]]
	const RenderingConfig& getRenderingConfig() const {
		return *rendering_config;
	}

	[[nodiscard]]
	ID3D11Device& getDevice() const {
		return direct3D->getDevice();
	}

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() const {
		return direct3D->getDeviceContext();
	}

	[[nodiscard]]
	SwapChain& getSwapChain() const {
		return *swap_chain;
	}

	[[nodiscard]]
	ResourceMgr& getResourceMgr() const {
		return *resource_mgr;
	}

	[[nodiscard]]
	const GPUProfiler& getProfiler() const {
		return renderer->getProfiler();
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::unique_ptr<Direct3D>        direct3D;
	std::unique_ptr<DisplayConfig>   display_config;
	std::unique_ptr<RenderingConfig> rendering_config;
	std::unique_ptr<SwapChain>       swap_chain;
	std::unique_ptr<ResourceMgr>     resource_mgr;
	std::unique_ptr<Renderer>        renderer;
};

} //namespace render