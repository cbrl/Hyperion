module;

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"
#include "hlsl.h"

export module rendering.renderer;

export import rendering.gpu_profiler;
import rendering.buffer_types;
import rendering.constant_buffer;
import rendering.output_mgr;
import rendering.render_state_mgr;


namespace render {

class DisplayConfig;
class ResourceMgr;
class RenderingMgr;
class RenderingConfig;
class Scene;
class SwapChain;

class LightPass;
class ForwardPass;
class DeferredPass;
class SkyPass;
class BoundingVolumePass;
class TextPass;

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