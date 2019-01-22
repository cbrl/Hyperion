#pragma once

#include "directx/d3d11.h"
#include "directx/profiler/gpu_profiler.h"
#include "hlsl.h"

#include "renderer/output/output_mgr.h"
#include "renderer/state/render_state_mgr.h"

#include "scene/scene.h"
#include "renderer/pass/light/light_pass.h"
#include "renderer/pass/forward/forward_pass.h"
#include "renderer/pass/sky/sky_pass.h"
#include "renderer/pass/bounding_volume/bounding_volume_pass.h"
#include "renderer/pass/text/text_pass.h"


class RenderingMgr;
class RenderingConfig;


class Renderer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Renderer(DisplayConfig& display_config,
	         const RenderingConfig& rendering_config,
	         ID3D11Device& device,
	         ID3D11DeviceContext& device_context,
	         SwapChain& swap_chain,
	         ResourceMgr& resource_mgr);

	Renderer(const Renderer& renderer) = delete;
	Renderer(Renderer&& renderer) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Renderer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Renderer& operator=(const Renderer& renderer) = delete;
	Renderer& operator=(Renderer&& renderer) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const GPUProfiler& getProfiler() const noexcept {
		return profiler;
	}

	void onResize() {
		output_mgr->resizeBuffers();
	}

	void render(Scene& scene);


private:
	template<typename CameraT>
	void renderCamera(Scene& scene, const CameraT& camera);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ID3D11Device&        device;
	ID3D11DeviceContext& device_context;

	GPUProfiler profiler;

	std::unique_ptr<OutputMgr>      output_mgr;
	std::unique_ptr<RenderStateMgr> render_state_mgr;

	// Renderers
	std::unique_ptr<LightPass>          light_pass;
	std::unique_ptr<ForwardPass>        forward_pass;
	std::unique_ptr<SkyPass>            sky_pass;
	std::unique_ptr<BoundingVolumePass> bounding_volume_pass;
	std::unique_ptr<TextPass>           text_pass;
};


//#include "renderer.tpp"
