#include "forward_pass.h"
#include "engine/engine.h"
#include "pipeline.h"
#include "hlsl.h"
#include "geometry/frustum/frustum.h"

#include "compiled_headers/forward.h"
#include "compiled_headers/forward_vs.h"


ForwardPass::ForwardPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context) {

	// Create the vertex shader
	vertex_shader = make_unique<VertexShader>(device,
	                                          shader_forward_vs,
	                                          sizeof(shader_forward_vs),
	                                          VertexPositionNormalTexture::InputElements,
	                                          VertexPositionNormalTexture::InputElementCount);

	// Create the pixel shader
	pixel_shader = make_unique<PixelShader>(device, shader_forward, sizeof(shader_forward));
}


void ForwardPass::bindRenderStates(const RenderStateMgr& render_state_mgr) const {
	
	// Bind topology
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind null shaders
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

	// Bind shaders
	pixel_shader->bind(device_context);
	vertex_shader->bind(device_context);

	// Bind render states
	render_state_mgr.bindOpaque(device_context);
	render_state_mgr.bindDepthDefault(device_context);
	render_state_mgr.bindCullCounterClockwise(device_context);
}


void XM_CALLCONV ForwardPass::render(const Engine& engine, FXMMATRIX world_to_projection) const {

	auto& ecs_engine = engine.getECS();
	const auto& render_state_mgr = engine.getRenderingMgr().getRenderStateMgr();

	// Bind the shaders, render states, etc
	bindRenderStates(render_state_mgr);

	// Render models
	ecs_engine.forEach<Model>([&](Model& model) {
		if (model.isActive())
			renderModel(ecs_engine, model, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::render(const Engine& engine, const Texture* sky, FXMMATRIX world_to_projection) const {

	auto& ecs_engine             = engine.getECS();
	const auto& render_state_mgr = engine.getRenderingMgr().getRenderStateMgr();

	// Bind the skybox texture if it has one
	if (sky) sky->bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);

	// Bind the shaders, render states, etc
	bindRenderStates(render_state_mgr);

	// Render models
	ecs_engine.forEach<Model>([&](Model& model) {
		if (model.isActive())
			renderModel(ecs_engine, model, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderModel(ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const {

	const auto transform = ecs_engine.getComponent<Transform>(model.getOwner());
	if (!transform) return;

	const auto model_to_world = transform->getObjectToWorldMatrix();
	const auto model_to_proj = model_to_world * world_to_projection;

	// Cull the model if it isn't on screen
	Frustum frustum(model_to_proj);
	if (!Frustum(model_to_proj).contains(model.getAABB()))
		return;

	// Bind the model's mesh
	model.bind(device_context);

	// Render each model part individually
	model.forEachChild([&](ModelChild& child) {

		if (!frustum.contains(child.getAABB()))
			return;

		// Bind the child model's buffer
		child.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
		child.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);

		// Get the child model's material
		const auto& mat = child.getMaterial();

		// Bind the SRVs
		if (mat.map_diffuse)        mat.map_diffuse->bind<Pipeline::PS>(device_context, SLOT_SRV_DIFFUSE);
		if (mat.map_ambient)        mat.map_ambient->bind<Pipeline::PS>(device_context, SLOT_SRV_AMBIENT);
		if (mat.map_specular)       mat.map_specular->bind<Pipeline::PS>(device_context, SLOT_SRV_SPECULAR);
		if (mat.map_spec_highlight) mat.map_spec_highlight->bind<Pipeline::PS>(device_context, SLOT_SRV_SPEC_HIGHLIGHT);
		if (mat.map_alpha)          mat.map_alpha->bind<Pipeline::PS>(device_context, SLOT_SRV_ALPHA);
		if (mat.map_bump)           mat.map_bump->bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);


		// Draw the child
		model.draw(device_context, child.getIndexCount(), child.getIndexStart());


		// Unbind the SRVs
		// Slot definition could be used as a more dynamic way of unbinding any amount of srvs
		// E.g. null_srv[SLOT_SRV_ALPHA + 1] = { nullptr };
		ID3D11ShaderResourceView* null_srv[6] = {nullptr};
		Pipeline::PS::bindSRVs(device_context, 0, 6, null_srv);
	});
}
