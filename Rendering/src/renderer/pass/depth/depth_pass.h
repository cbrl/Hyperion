#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "components/model/model.h"

class RenderStateMgr;
class ResourceMgr;
class Scene;

class DepthPass final {
public:
	DepthPass(ID3D11Device& device,
	          ID3D11DeviceContext& device_context,
	          RenderStateMgr& render_state_mgr,
	          ResourceMgr& resource_mgr);
	~DepthPass() = default;

	void bindState() const;

	void XM_CALLCONV render(Scene& scene,
	                        FXMMATRIX world_to_camera,
	                        CXMMATRIX camera_to_projection) const;

	void XM_CALLCONV renderShadows(Scene& scene,
	                               FXMMATRIX world_to_camera,
	                               CXMMATRIX camera_to_projection) const;


private:
	void XM_CALLCONV updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const;
	void XM_CALLCONV renderModel(const Model& model, FXMMATRIX model_to_projection) const;


private:
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;

	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;

	ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};
