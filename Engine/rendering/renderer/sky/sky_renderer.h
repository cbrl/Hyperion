#pragma once

#include <d3d11.h>

#include "util\datatypes\datatypes.h"
#include "shader\hlsl.h"
#include "shader\shader.h"

class Engine;
class RenderStateMgr;
class Scene;

class SkyRenderer final {
	public:
		SkyRenderer(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~SkyRenderer() = default;

		void Render(const Engine& engine);


	private:
		void BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr);


	private:
		reference_wrapper<ID3D11Device>        device;
		reference_wrapper<ID3D11DeviceContext> device_context;

		unique_ptr<VertexShader> vertex_shader;
		unique_ptr<PixelShader>  pixel_shader;
};