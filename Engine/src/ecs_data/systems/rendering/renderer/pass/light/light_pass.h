#pragma once

#include "rendering/buffer/buffers.h"
#include "rendering/buffer/constant_buffer.h"
#include "rendering/buffer/structured_buffer.h"
#include "rendering/buffer/shadow_map_buffer.h"
#include "ecs_data/components/rendering/light/directional_light.h"
#include "ecs_data/components/rendering/light/point_light.h"
#include "ecs_data/components/rendering/light/spot_light.h"
#include "ecs_data/systems/rendering/renderer/pass/depth/depth_pass.h"
#include "scene/scene.h"

class Engine;


class LightPass final {
public:
	LightPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~LightPass() = default;

	void XM_CALLCONV Render(const Engine& engine, FXMMATRIX world_to_projection);


private:
	void BindBuffers();

	void UpdateShadowMaps();
	void RenderShadowMaps(const Engine& engine);

	void UpdateData(Scene& scene) const;
	void XM_CALLCONV UpdateDirectionalLightData(ECS& ecs_engine);
	void XM_CALLCONV UpdatePointLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);
	void XM_CALLCONV UpdateSpotLightData(ECS& ecs_engine, FXMMATRIX world_to_projection);


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
	StructuredBuffer<ShadowedDirectionalLightBuffer> shadowed_directional_lights;
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
