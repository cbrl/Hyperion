#pragma once

#include "buffer/constant_buffer.h"
#include "shader/shader.h"
#include "geometry/bounding_volume/bounding_volume.h"

class Engine;
class RenderStateMgr;

class BoundingVolumePass final {
public:
	BoundingVolumePass(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~BoundingVolumePass() = default;

	void bindRenderStates(const RenderStateMgr& render_state_mgr) const;
	void XM_CALLCONV render(const Engine& engine, FXMMATRIX world_to_projection) const;


private:
	void XM_CALLCONV renderAABB(const AABB& aabb, FXMMATRIX object_to_world) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	unique_ptr<VertexShader> vertex_shader;
	unique_ptr<PixelShader>  pixel_shader;

	ConstantBuffer<XMMATRIX> model_matrix_buffer;
};