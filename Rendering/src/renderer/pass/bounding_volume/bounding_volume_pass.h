#pragma once

#include "buffer/constant_buffer.h"
#include "resource/shader/shader.h"
#include "geometry/bounding_volume/bounding_volume.h"

class Transform;

namespace ecs {
	class ECS;
}

namespace render {

class RenderStateMgr;
class ResourceMgr;

class BoundingVolumePass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	BoundingVolumePass(ID3D11Device& device,
	                   ID3D11DeviceContext& device_context,
	                   RenderStateMgr& render_state_mgr,
	                   ResourceMgr& resource_mgr);

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~BoundingVolumePass() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	BoundingVolumePass& operator=(const BoundingVolumePass&) = delete;
	BoundingVolumePass& operator=(BoundingVolumePass&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void XM_CALLCONV render(const ecs::ECS& ecs, FXMMATRIX world_to_projection, const f32_4& color) const;

private:

	void bindRenderStates() const;
	void XM_CALLCONV renderAABB(const AABB& aabb, const Transform& transform, FXMMATRIX world_to_projection) const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

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

} //namespace render
