#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"

class RenderStateMgr;
class ResourceMgr;
class Scene;
class Model;

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
	void bindOpaqueShaders() const;
	void bindTransparentShaders() const;

	void XM_CALLCONV updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const;

	void XM_CALLCONV renderModel(const Model& model, FXMMATRIX world_to_projection) const;


private:
	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	shared_ptr<VertexShader> opaque_vs;
	shared_ptr<VertexShader> transparent_vs;
	shared_ptr<PixelShader>  transparent_ps;

	// Buffers
	ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};
