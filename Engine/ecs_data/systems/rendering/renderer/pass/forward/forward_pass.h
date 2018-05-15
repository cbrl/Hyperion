#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\buffer\structured_buffer.h"
#include "shader\hlsl.h"
#include "shader\shader.h"

class ECS;
class Engine;
class Scene;
class RenderStateMgr;

class ForwardPass final {
	public:
		ForwardPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~ForwardPass() = default;

		void Render(const Engine& engine);


	private:
		void BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr) const;
		void UpdateLightBuffers(ECS& ecs_engine, Scene& scene);
		void RenderModels(ECS& ecs_engine, Scene& scene) const;


	private:
		reference_wrapper<ID3D11Device>        device;
		reference_wrapper<ID3D11DeviceContext> device_context;

		unique_ptr<PixelShader>  pixel_shader;
		unique_ptr<VertexShader> vertex_shader;

		ConstantBuffer<LightBuffer>              light_buffer;
		StructuredBuffer<DirectionalLightBuffer> directional_light_buffer;
		StructuredBuffer<PointLightBuffer>       point_light_buffer;
		StructuredBuffer<SpotLightBuffer>        spot_light_buffer;
};