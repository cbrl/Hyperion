#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"
#include "rendering\render_state_mgr.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "shader\shader.h"
#include "ecs_data\components\rendering\model\model.h"

class Engine;

class DepthPass final {
	public:
		DepthPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~DepthPass() = default;

		void BindState(const RenderStateMgr& render_state_mgr) const;

		void XM_CALLCONV Render(const Engine& engine,
								FXMMATRIX world_to_camera,
								CXMMATRIX camera_to_projection);

		void XM_CALLCONV RenderShadows(const Engine& engine,
									   FXMMATRIX world_to_camera,
									   CXMMATRIX camera_to_projection);


	private:
		void XM_CALLCONV UpdateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection);
		void XM_CALLCONV RenderModel(Model& model, FXMMATRIX model_to_projection);


	private:
		reference_wrapper<ID3D11Device>        device;
		reference_wrapper<ID3D11DeviceContext> device_context;

		unique_ptr<VertexShader> vertex_shader;
		unique_ptr<PixelShader>  pixel_shader;

		ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};