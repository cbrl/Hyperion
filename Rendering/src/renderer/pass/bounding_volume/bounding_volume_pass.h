#pragma once

#include "buffer/constant_buffer.h"
#include "resource/shader/shader.h"
#include "geometry/bounding_volume/bounding_volume.h"

class RenderStateMgr;
class ResourceMgr;
class Scene;

class BoundingVolumePass final {
public:
	BoundingVolumePass(ID3D11Device& device,
	                   ID3D11DeviceContext& device_context,
	                   RenderStateMgr& render_state_mgr,
	                   ResourceMgr& resource_mgr);
	~BoundingVolumePass() = default;

	void bindRenderStates() const;
	void XM_CALLCONV render(Scene& scene, FXMMATRIX world_to_projection) const;


private:
	void XM_CALLCONV renderAABB(const AABB& aabb, FXMMATRIX object_to_world) const;


private:
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr> render_state_mgr;

	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader>  pixel_shader;

	ConstantBuffer<XMMATRIX> model_matrix_buffer;
};