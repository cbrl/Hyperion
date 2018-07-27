#pragma once

#include "datatypes/datatypes.h"
#include "resource/shader/shader.h"
#include "components/model/model.h"
#include "resource/shader/shader_factory.h"

class RenderStateMgr;
class ResourceMgr;
class ECS;
class Scene;

class ForwardPass final {
public:
	ForwardPass(ID3D11Device& device,
	            ID3D11DeviceContext& device_context,
	            RenderStateMgr& render_state_mgr,
	            ResourceMgr& resource_mgr);
	~ForwardPass() = default;

	void XM_CALLCONV render(Scene& scene, FXMMATRIX world_to_projection, const Texture* sky) const;
	void XM_CALLCONV renderUnlit(Scene& scene, FXMMATRIX world_to_projection, const Texture* sky) const;
	void XM_CALLCONV renderFalseColor(Scene& scene, FXMMATRIX world_to_projection, FalseColor color) const;
	void XM_CALLCONV renderWireframe(Scene& scene, FXMMATRIX world_to_projection) const;


private:
	void bindDefaultState() const;
	void bindWireframeState() const;
	void XM_CALLCONV renderModel(ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const;


private:
	std::reference_wrapper<ID3D11DeviceContext> device_context;
	std::reference_wrapper<RenderStateMgr>      render_state_mgr;
	std::reference_wrapper<ResourceMgr>         resource_mgr;

	ConstantBuffer<vec4_f32> color_buffer;
	shared_ptr<VertexShader> vertex_shader;
};
