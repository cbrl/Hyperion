module;

#include <chrono>
#include <memory>

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

#include "hlsl.h"
#include "directx/d3d11.h"

#include "rendering_forward_decs.h"

export module rendering:renderer;

import :gpu_profiler;
import :buffer_types;
import :constant_buffer;
import :output_mgr;
import :render_state_mgr;
import :rendering_config;

using namespace DirectX;

namespace render {

export class Renderer final {
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
	~Renderer();


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

	void render(Scene& scene, std::chrono::duration<f32> delta_time);

private:

	void updateBuffers(std::chrono::duration<f32> delta_time);

	template<typename CameraT>
	void renderCamera(Scene& scene, const CameraT& camera);

	template<typename CameraT>
	void XM_CALLCONV renderForward(Scene& scene,
	                               CameraT& camera,
	                               FXMMATRIX world_to_projection);

	template<typename CameraT>
	void XM_CALLCONV renderDeferred(Scene& scene,
	                                CameraT& camera,
	                                FXMMATRIX world_to_projection);

	template<typename CameraT>
	void XM_CALLCONV renderFalseColor(Scene& scene,
	                                  CameraT& camera,
	                                  FXMMATRIX world_to_projection);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Device/Context References
	ID3D11Device&        device;
	ID3D11DeviceContext& device_context;

	DisplayConfig& display_config;

	// State Managers
	std::unique_ptr<OutputMgr>      output_mgr;
	std::unique_ptr<RenderStateMgr> render_state_mgr;

	// Profiles render times for different render stages
	GPUProfiler profiler;

	// Buffers
	ConstantBuffer<EngineBuffer> engine_buffer;

	// Renderers
	std::unique_ptr<LightPass>          light_pass;
	std::unique_ptr<ForwardPass>        forward_pass;
	std::unique_ptr<DeferredPass>       deferred_pass;
	std::unique_ptr<SkyPass>            sky_pass;
	std::unique_ptr<BoundingVolumePass> bounding_volume_pass;
	std::unique_ptr<TextPass>           text_pass;
};

} //namespace render