#include "stdafx.h"
#include "forward_pass.h"
#include "rendering\render_state_mgr.h"
#include "scene\scene.h"

#include "compiled_headers\forward.h"
#include "compiled_headers\forward_vs.h"

ForwardPass::ForwardPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
{

	// Create the vertex shader
	vertex_shader = make_unique<VertexShader>(device,
											  shader_forward_vs,
											  sizeof(shader_forward_vs),
											  VertexPositionNormalTexture::InputElements,
											  VertexPositionNormalTexture::InputElementCount);

	// Create the pixel shader
	pixel_shader = make_unique<PixelShader>(device, shader_forward, sizeof(shader_forward));
}


void XM_CALLCONV ForwardPass::Render(ECS& ecs_engine, const FXMMATRIX world_to_projection) const {

	//auto skybox = ecs_engine.GetComponent<SkyBox>(camera.GetHandle());
	//if (skybox) {
	//	skybox->GetTexture()->Bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);
	//}

	// Bind shaders
	pixel_shader->Bind(device_context);
	vertex_shader->Bind(device_context);


	// Render models
	ecs_engine.ForEachActive<Model>([&](Model& model) {
		Render(ecs_engine, model, world_to_projection);
	});
}


void ForwardPass::BindDefaultRenderStates(const RenderStateMgr& render_state_mgr) const {

	Pipeline::DS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::BindShader(device_context, nullptr, nullptr, 0);

	render_state_mgr.BindOpaque(device_context);
	render_state_mgr.BindDepthDefault(device_context);
	render_state_mgr.BindCullCounterClockwise(device_context);
}


void XM_CALLCONV ForwardPass::Render(const ECS& ecs_engine, Model& model, FXMMATRIX world_to_projection) const {

	const auto transform            = ecs_engine.GetComponent<Transform>(model.GetOwner());
	const auto object_to_world      = transform->GetObjectToWorldMatrix();
	const auto object_to_projection = object_to_world * world_to_projection;

	// Cull the model if it isn't on screen
	Frustum frustum(object_to_projection);
	if (!frustum.Contains(model.GetAABB())) return;

	// Bind the model's mesh
	model.Bind(device_context);

	// Render each model part individually
	model.ForEachChild([&](ModelChild& child) {

		if (!frustum.Contains(child.GetAABB())) return;

		// Bind the child model's buffer
		child.BindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
		child.BindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);

		// Get the child model's material
		const auto& mat = child.GetMaterial();

		// Bind the SRVs
		if (mat.map_diffuse)        mat.map_diffuse->Bind<Pipeline::PS>(device_context, SLOT_SRV_DIFFUSE);
		if (mat.map_ambient)        mat.map_ambient->Bind<Pipeline::PS>(device_context, SLOT_SRV_AMBIENT);
		if (mat.map_specular)       mat.map_specular->Bind<Pipeline::PS>(device_context, SLOT_SRV_SPECULAR);
		if (mat.map_spec_highlight) mat.map_spec_highlight->Bind<Pipeline::PS>(device_context, SLOT_SRV_SPEC_HIGHLIGHT);
		if (mat.map_alpha)          mat.map_alpha->Bind<Pipeline::PS>(device_context, SLOT_SRV_ALPHA);
		if (mat.map_bump)           mat.map_bump->Bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);


		// Draw the child
		model.Draw(device_context, child.GetIndexCount(), child.GetIndexStart());


		// Unbind the SRVs
		// Slot definition could be used as a more dynamic way of unbinding any amount of srvs
		// E.g. null_srv[SLOT_SRV_ALPHA + 1] = { nullptr };
		ID3D11ShaderResourceView* null_srv[6] = { nullptr };
		Pipeline::PS::BindSRVs(device_context, 0, 6, null_srv);
	});
}