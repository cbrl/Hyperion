#pragma once

#include <d3d11.h>
#include <DirectXColors.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "direct3d\direct3d.h"
#include "rendering\pipeline.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\render_state_mgr.h"
#include "rendering\forward\forward_render.h"
#include "scene\scene.h"


class Renderer {
	public:
		Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~Renderer() = default;

		void Render(Scene& scene, RenderStateMgr& render_state_mgr) const;


	private:
		ComPtr<ID3D11DeviceContext> device_context;

		// Renderers
		unique_ptr<ForwardRenderer> forward_renderer;

		// Buffers
		ConstantBuffer<CameraBuffer> camera_buffer;
};