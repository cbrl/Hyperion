#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"

class Model;

namespace render {

class RenderStateMgr;
class ResourceMgr;
class Scene;

class DepthPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	DepthPass(ID3D11Device& device,
	          ID3D11DeviceContext& device_context,
	          RenderStateMgr& render_state_mgr,
	          ResourceMgr& resource_mgr);

	DepthPass(const DepthPass&) = delete;
	DepthPass(DepthPass&&) = default;
	

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~DepthPass() = default;

	
	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	DepthPass& operator=(const DepthPass&) = delete;
	DepthPass& operator=(DepthPass&&) = default;

	
	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
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

	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	std::shared_ptr<VertexShader> opaque_vs;
	std::shared_ptr<VertexShader> transparent_vs;
	std::shared_ptr<PixelShader>  transparent_ps;

	// Buffers
	ConstantBuffer<AltCameraBuffer> alt_cam_buffer;
};

} //namespace render
