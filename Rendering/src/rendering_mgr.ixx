module;

#include <chrono>
#include <memory>
#include <utility>

#include "datatypes/pointer_types.h"
#include "directx/d3d11.h"

export module rendering:rendering_mgr;

import :display_config;
import :gpu_profiler;
import :rendering_config;
import :renderer;
import :resource_mgr;
import :swapchain;

namespace render {

export class RenderingMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	RenderingMgr(gsl::not_null<HWND> window,
	             DisplayConfig display_conf,
	             RenderingConfig rendering_conf);

	RenderingMgr(const RenderingMgr& mgr) = delete;
	RenderingMgr(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~RenderingMgr();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	RenderingMgr& operator=(const RenderingMgr& mgr) = delete;
	RenderingMgr& operator=(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Resizing
	//----------------------------------------------------------------------------------

	// Resize the buffers
	void onResize() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Rendering
	//----------------------------------------------------------------------------------

	// Start a new frame
	void beginFrame() const;

	// End the current frame
	void endFrame() const;

	// Render the current scene
	void render(Scene& scene, std::chrono::duration<f32> delta_time) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Getters
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DisplayConfig& getDisplayConfig();

	[[nodiscard]]
	const DisplayConfig& getDisplayConfig() const;

	[[nodiscard]]
	RenderingConfig& getRenderingConfig();

	[[nodiscard]]
	const RenderingConfig& getRenderingConfig() const;

	[[nodiscard]]
	ID3D11Device& getDevice() const;

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() const;

	[[nodiscard]]
	SwapChain& getSwapChain() const;

	[[nodiscard]]
	ResourceMgr& getResourceMgr() const;

	[[nodiscard]]
	const GPUProfiler& getProfiler() const;


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
