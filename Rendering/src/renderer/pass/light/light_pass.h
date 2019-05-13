#pragma once

#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "buffer/structured_buffer.h"
#include "buffer/shadow_map_buffer.h"
#include "renderer/pass/depth/depth_pass.h"


namespace ecs { class ECS; }

namespace render {

class RenderingConfig;
class RenderStateMgr;
class ResourceMgr;
class Scene;

class LightPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	LightPass(const RenderingConfig& rendering_config,
	          ID3D11Device& device,
	          ID3D11DeviceContext& device_context,
	          RenderStateMgr& render_state_mgr,
	          ResourceMgr& resource_mgr);

	LightPass(const LightPass&) = delete;
	LightPass(LightPass&&) = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~LightPass() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	LightPass& operator=(const LightPass&) = delete;
	LightPass& operator=(LightPass&&) = default;

	
	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void XM_CALLCONV render(const Scene& scene, FXMMATRIX world_to_projection);

private:

	void bindBuffers();

	void updateShadowMaps();
	void renderShadowMaps(const Scene& scene);

	void updateData(const Scene& scene) const;
	void XM_CALLCONV updateDirectionalLightData(const Scene& scene, FXMMATRIX world_to_projection);
	void XM_CALLCONV updatePointLightData(const Scene& scene, FXMMATRIX world_to_projection);
	void XM_CALLCONV updateSpotLightData(const Scene& scene, FXMMATRIX world_to_projection);

	
	//----------------------------------------------------------------------------------
	// Camera definition for rendering from light's POV
	//----------------------------------------------------------------------------------
	struct LightCamera {
		XMMATRIX world_to_light;
		XMMATRIX light_to_proj;
	};

	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	ID3D11Device&          device;
	ID3D11DeviceContext&   device_context;
	const RenderingConfig& rendering_config;

	// Depth rendering pass
	std::unique_ptr<DepthPass> depth_pass;

	// Light info buffer
	ConstantBuffer<LightBuffer> light_buffer;

	// Light buffers
	StructuredBuffer<DirectionalLightBuffer> directional_lights;
	StructuredBuffer<PointLightBuffer>       point_lights;
	StructuredBuffer<SpotLightBuffer>        spot_lights;

	// Shadowed light buffers
	StructuredBuffer<DirectionalLightBuffer> shadowed_directional_lights;
	StructuredBuffer<ShadowedPointLightBuffer> shadowed_point_lights;
	StructuredBuffer<ShadowedSpotLightBuffer> shadowed_spot_lights;

	// Light cameras
	std::vector<LightCamera> directional_light_cameras;
	std::vector<LightCamera> point_light_cameras;
	std::vector<LightCamera> spot_light_cameras;

	// Shadow maps
	std::unique_ptr<ShadowMapBuffer>     directional_light_smaps;
	std::unique_ptr<ShadowCubeMapBuffer> point_light_smaps;
	std::unique_ptr<ShadowMapBuffer>     spot_light_smaps;
};

} //namespace render
