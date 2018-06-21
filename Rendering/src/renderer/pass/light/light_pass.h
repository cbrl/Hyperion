#pragma once

#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "buffer/structured_buffer.h"
#include "buffer/shadow_map_buffer.h"
#include "components/rendering/light/directional_light.h"
#include "components/rendering/light/point_light.h"
#include "components/rendering/light/spot_light.h"
#include "renderer/pass/depth/depth_pass.h"
#include "hlsl.h"

class ECS;
class Scene;

class LightPass final {
public:
	LightPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~LightPass() = default;

	void XM_CALLCONV render(const Engine& engine, FXMMATRIX world_to_projection);


private:
	void bindBuffers();

	void updateShadowMaps();
	void renderShadowMaps(const Engine& engine);

	void updateData(Scene& scene) const;
	void XM_CALLCONV updateDirectionalLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);
	void XM_CALLCONV updatePointLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);
	void XM_CALLCONV updateSpotLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);


private:
	struct LightCamera {
		XMMATRIX world_to_light;
		XMMATRIX light_to_proj;
	};


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

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
	vector<LightCamera> directional_light_cameras;
	vector<LightCamera> point_light_cameras;
	vector<LightCamera> spot_light_cameras;

	// Shadow maps
	unique_ptr<ShadowMapBuffer> directional_light_smaps;
	unique_ptr<ShadowCubeMapBuffer> point_light_smaps;
	unique_ptr<ShadowMapBuffer> spot_light_smaps;
};
