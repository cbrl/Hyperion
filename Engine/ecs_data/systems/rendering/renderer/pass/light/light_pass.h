#pragma once

#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\buffer\structured_buffer.h"
#include "ecs_data\components\rendering\light\directional_light.h"
#include "ecs_data\components\rendering\light\point_light.h"
#include "ecs_data\components\rendering\light\spot_light.h"
#include "scene\scene.h"

class Engine;

class LightPass final {
	public:
		LightPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~LightPass() = default;

		void Render(const Engine& engine);
		void BindBuffers();

		void UpdateData(ECS& ecs_engine, Scene& scene) const;
		void UpdateDirectionalLightData(ECS& ecs_engine);
		void UpdatePointLightData(ECS& ecs_engine);
		void UpdateSpotLightData(ECS& ecs_engine);


	private:
		reference_wrapper<ID3D11Device>        device;
		reference_wrapper<ID3D11DeviceContext> device_context;

		ConstantBuffer<LightBuffer>              light_buffer;
		StructuredBuffer<DirectionalLightBuffer> directional_light_buffer;
		StructuredBuffer<PointLightBuffer>       point_light_buffer;
		StructuredBuffer<SpotLightBuffer>        spot_light_buffer;
};