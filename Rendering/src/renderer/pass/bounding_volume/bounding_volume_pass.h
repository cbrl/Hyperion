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

	void XM_CALLCONV render(Scene& scene, FXMMATRIX world_to_projection, const f32_4& color) const;


private:
	void bindRenderStates() const;
	void XM_CALLCONV renderAABB(const AABB& aabb, FXMMATRIX object_to_world) const;


private:
	// Dependency References
	ID3D11DeviceContext& device_context;
	RenderStateMgr&      render_state_mgr;

	// Shaders
	std::shared_ptr<VertexShader> vertex_shader;
	std::shared_ptr<PixelShader> pixel_shader;

	// Buffers
	ConstantBuffer<XMMATRIX> model_matrix_buffer;
	ConstantBuffer<f32_4> color_buffer;
};