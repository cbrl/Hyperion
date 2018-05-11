#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "rendering\render_state_mgr.h"
#include "resource\resource_mgr.h"
#include "shader\hlsl.h"
#include "shader\shader.h"
#include "scene\scene.h"


class SkyRenderer final {
	public:
		SkyRenderer(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~SkyRenderer() = default;

		void Render(Scene& scene, const RenderStateMgr& render_state_mgr);


	private:
		void BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr);


	private:
		reference_wrapper<ID3D11Device>        device;
		reference_wrapper<ID3D11DeviceContext> device_context;

		unique_ptr<VertexShader> vertex_shader;
		unique_ptr<PixelShader>  pixel_shader;
};