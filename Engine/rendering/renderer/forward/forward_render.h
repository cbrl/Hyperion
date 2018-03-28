#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\buffer\structured_buffer.h"
#include "rendering\render_state_mgr.h"
#include "shader\shader.h"
#include "scene\scene.h"


class ForwardRenderer {
	public:
		ForwardRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~ForwardRenderer() = default;

		void Render(Scene& scene, RenderStateMgr& render_state_mgr);


	private:
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> device_context;

		unique_ptr<PixelShader>  pixel_shader;
		unique_ptr<VertexShader> vertex_shader;

		ConstantBuffer<LightBuffer>        light_buffer;
		StructuredBuffer<DirectionalLight> directional_light_buffer;
		StructuredBuffer<PointLight>       point_light_buffer;
		StructuredBuffer<SpotLight>        spot_light_buffer;
};