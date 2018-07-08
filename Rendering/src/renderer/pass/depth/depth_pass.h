#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "components/model/model.h"

class Engine;
class RenderStateMgr;

class DepthPass final {
public:
	DepthPass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~DepthPass() = default;

	void bindState(const RenderStateMgr& render_state_mgr) const;

	void XM_CALLCONV render(const Engine& engine,
	                        FXMMATRIX world_to_camera,
	                        CXMMATRIX camera_to_projection) const;

	void XM_CALLCONV renderShadows(const Engine& engine,
	                               FXMMATRIX world_to_camera,
	                               CXMMATRIX camera_to_projection) const;


private:
	void XM_CALLCONV updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const;
	void XM_CALLCONV renderModel(const Model& model, FXMMATRIX model_to_projection) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<VertexShader> vertex_shader;
	unique_ptr<PixelShader> pixel_shader;

	ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};
