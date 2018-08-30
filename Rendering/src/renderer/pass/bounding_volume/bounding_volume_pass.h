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

	void XM_CALLCONV render(Scene& scene, FXMMATRIX world_to_projection, const vec4_f32& color) const;


private:
	void bindRenderStates() const;
	void XM_CALLCONV renderAABB(const AABB& aabb, FXMMATRIX object_to_world) const;


private:
	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader>  pixel_shader;

	// Buffers
	ConstantBuffer<XMMATRIX> model_matrix_buffer;
	ConstantBuffer<vec4_f32> color_buffer;
};