#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "components/model/model.h"

class ECS;
class Engine;
class RenderStateMgr;

class ForwardPass final {
public:
	ForwardPass(ID3D11Device& device,
	            ID3D11DeviceContext& device_context,
	            RenderStateMgr& render_state_mgr,
	            ResourceMgr& resource_mgr);
	~ForwardPass() = default;

	void XM_CALLCONV render(const Engine& engine, FXMMATRIX world_to_projection) const;
	void XM_CALLCONV render(const Engine& engine, const Texture* sky, FXMMATRIX world_to_projection) const;


private:
	void bindRenderStates() const;
	void XM_CALLCONV renderModel(ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const;


private:
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;
	reference_wrapper<RenderStateMgr> render_state_mgr;

	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<VertexShader> vertex_shader;
};
