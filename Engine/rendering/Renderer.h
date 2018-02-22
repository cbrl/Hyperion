#pragma once

#include <memory>
#include <wrl\client.h>
#include <d3d11.h>
#include <DirectXColors.h>

#include "util\engine_util.h"
#include "geometry\mesh\vertex_types.h"
#include "direct3d\direct3d.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "scene\scene.h"
#include "rendering\forward\forward_render.h"


using std::unique_ptr;
using Microsoft::WRL::ComPtr;


class Renderer {
	public:
		static const Renderer* Get();

		Renderer();
		~Renderer();

		void Tick(Scene& scene, float deltaTime) const;


	private:
		// Renderers
		unique_ptr<ForwardRenderer> forward_renderer;

		// Buffers
		ConstantBuffer<CameraBuffer> camera_buffer;
};