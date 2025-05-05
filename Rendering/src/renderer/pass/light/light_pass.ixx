module;

#include <memory>
#include <vector>

#include <DirectXMath.h>

#include "datatypes/types.h"
#include "directx/d3d11.h"

export module rendering:pass.light_pass;

import ecs;
import :buffer_types;
import :constant_buffer;
import :pass.depth_pass;
import :render_state_mgr;
import :rendering_config;
import :resource_mgr;
import :shadow_map_buffer;
import :structured_buffer;

using namespace DirectX;

namespace render {

export class LightPass final {
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
	void XM_CALLCONV render(const ecs::ECS& ecs, FXMMATRIX world_to_projection);

private:

	void bindBuffers();

	void updateShadowMaps();
	void renderShadowMaps(const ecs::ECS& ecs);

	void updateData(const ecs::ECS& ecs) const;
	void XM_CALLCONV updateDirectionalLightData(const ecs::ECS& ecs, FXMMATRIX world_to_projection);
	void XM_CALLCONV updatePointLightData(const ecs::ECS& ecs, FXMMATRIX world_to_projection);
	void XM_CALLCONV updateSpotLightData(const ecs::ECS& ecs, FXMMATRIX world_to_projection);

	
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
