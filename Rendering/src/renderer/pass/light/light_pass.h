#pragma once

#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "buffer/structured_buffer.h"
#include "buffer/shadow_map_buffer.h"
#include "renderer/pass/depth/depth_pass.h"

class RenderStateMgr;
class ResourceMgr;
class ECS;
class Scene;

class LightPass final {
public:
	LightPass(ID3D11Device& device,
	          ID3D11DeviceContext& device_context,
	          RenderStateMgr& render_state_mgr,
	          ResourceMgr& resource_mgr);
	~LightPass() = default;

	void XM_CALLCONV render(Scene& scene, FXMMATRIX world_to_projection);


private:
	void bindBuffers();

	void updateShadowMaps();
	void renderShadowMaps(Scene& scene);

	void updateData() const;
	void XM_CALLCONV updateDirectionalLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);
	void XM_CALLCONV updatePointLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);
	void XM_CALLCONV updateSpotLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);


private:
	struct LightCamera {
		XMMATRIX world_to_light;
		XMMATRIX light_to_proj;
	};


private:
	std::reference_wrapper<ID3D11Device> device;
	std::reference_wrapper<ID3D11DeviceContext> device_context;

	// Depth rendering pass
	unique_ptr<DepthPass> depth_pass;

	// Light info buffer
	ConstantBuffer<LightBuffer> light_buffer;

	// Light buffers
	StructuredBuffer<DirectionalLightBuffer> directional_lights;
	StructuredBuffer<PointLightBuffer> point_lights;
	StructuredBuffer<SpotLightBuffer> spot_lights;

	// Shadowed light buffers
	StructuredBuffer<DirectionalLightBuffer> shadowed_directional_lights;
	StructuredBuffer<ShadowedPointLightBuffer> shadowed_point_lights;
	StructuredBuffer<ShadowedSpotLightBuffer> shadowed_spot_lights;

	// Light cameras
	std::vector<LightCamera> directional_light_cameras;
	std::vector<LightCamera> point_light_cameras;
	std::vector<LightCamera> spot_light_cameras;

	// Shadow maps
	unique_ptr<ShadowMapBuffer> directional_light_smaps;
	unique_ptr<ShadowCubeMapBuffer> point_light_smaps;
	unique_ptr<ShadowMapBuffer> spot_light_smaps;
};
