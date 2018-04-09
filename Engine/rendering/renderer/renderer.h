#pragma once

#include <d3d11.h>
#include <DirectXColors.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "shader\hlsl.h"
#include "rendering\pipeline.h"
#include "rendering\render_state_mgr.h"
#include "rendering\renderer\forward\forward_render.h"
#include "rendering\renderer\sky\sky_renderer.h"
#include "scene\scene.h"


class Renderer final {
	public:
		Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~Renderer() = default;

		void Render(System& system, RenderStateMgr& render_state_mgr) const;


	private:
		ComPtr<ID3D11DeviceContext> device_context;

		// Renderers
		unique_ptr<ForwardRenderer> forward_renderer;
		unique_ptr<SkyRenderer>     sky_renderer;
};