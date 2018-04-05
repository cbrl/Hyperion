#pragma once

#include <d3d11.h>
#include <DirectXColors.h>

#include "util\datatypes\datatypes.h"
#include "rendering\render_state_mgr.h"
#include "scene\scene.h"
#include "rendering\renderer\ui\ui_renderer.h"
#include "rendering\renderer\forward\forward_render.h"
#include "rendering\renderer\sky\sky_renderer.h"


class Renderer final {
	public:
		Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~Renderer() = default;

		void Render(Scene& scene, RenderStateMgr& render_state_mgr) const;


	private:
		ComPtr<ID3D11DeviceContext> device_context;

		// Renderers
		unique_ptr<UIRenderer>      ui_renderer;
		unique_ptr<ForwardRenderer> forward_renderer;
		unique_ptr<SkyRenderer>     sky_renderer;
};