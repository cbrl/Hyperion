#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\buffer\structured_buffer.h"
#include "rendering\render_state_mgr.h"
#include "shader\shader.h"
#include "scene\scene.h"


class ForwardRenderer final {
	public:
		ForwardRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~ForwardRenderer() = default;

		void Render(Scene& scene, const RenderStateMgr& render_state_mgr);


	private:
		void BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr) const;
		void UpdateLightBuffers(Scene& scene);
		void RenderModels(Scene& scene) const;


	private:
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> device_context;

		unique_ptr<PixelShader>  pixel_shader;
		unique_ptr<VertexShader> vertex_shader;

		ConstantBuffer<LightBuffer>              light_buffer;
		StructuredBuffer<DirectionalLightBuffer> directional_light_buffer;
		StructuredBuffer<PointLightBuffer>       point_light_buffer;
		StructuredBuffer<SpotLightBuffer>        spot_light_buffer;
};