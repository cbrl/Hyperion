#include "bounding_volume_pass.h"
#include "engine/engine.h"
#include "hlsl.h"
#include "geometry/frustum/frustum.h"

#include "compiled_headers/wireframe_box_vs.h"
#include "compiled_headers/wireframe_box_ps.h"


BoundingVolumePass::BoundingVolumePass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
	, model_matrix_buffer(device) {

	vertex_shader = make_unique<VertexShader>(L"shader_wireframe_box_vs",
	                                          device,
	                                          ShaderBytecodeBuffer(shader_wireframe_box_vs,
	                                                               sizeof(shader_wireframe_box_vs)),
	                                          nullptr,
	                                          0);

	pixel_shader = make_unique<PixelShader>(L"shader_wireframe_box_ps",
	                                        device, 
	                                        ShaderBytecodeBuffer(shader_wireframe_box_ps,
	                                                             sizeof(shader_wireframe_box_ps)));
}


void BoundingVolumePass::bindRenderStates(const RenderStateMgr& render_state_mgr) const {
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Unbind shaders
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

	// Bind the vertex and pixel shaders
	vertex_shader->bind(device_context);
	pixel_shader->bind(device_context);

	// Bind the constant buffer
	model_matrix_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

	// Bind the render states
	render_state_mgr.bindDepthDefault(device_context);
	render_state_mgr.bindOpaque(device_context);
}


void XM_CALLCONV BoundingVolumePass::render(const Engine& engine, FXMMATRIX world_to_projection) const {
	
	auto& ecs_engine          = engine.getECS();
	const auto& rendering_mgr = engine.getRenderingMgr().getRenderStateMgr();

	// Bind the render states
	bindRenderStates(rendering_mgr);


	ecs_engine.forEach<DirectionalLight>([&](const DirectionalLight& light) {

		if (!light.isActive()) return;
		
		const auto transform = ecs_engine.getComponent<Transform>(light.getOwner());
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getAABB()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<PointLight>([&](const PointLight& light) {

		if (!light.isActive()) return;

		const auto transform = ecs_engine.getComponent<Transform>(light.getOwner());
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getBoundingSphere()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<SpotLight>([&](const SpotLight& light) {

		if (!light.isActive()) return;

		const auto transform = ecs_engine.getComponent<Transform>(light.getOwner());
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getAABB()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<Model>([&](const Model& model) {

		if (!model.isActive()) return;

		const auto transform = ecs_engine.getComponent<Transform>(model.getOwner());
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(model.getAABB()))
			return;

		renderAABB(model.getAABB(), object_to_world);

		model.forEachChild([&](const ModelChild& child) {
			renderAABB(child.getAABB(), object_to_world);
		});
	});
}


void XM_CALLCONV BoundingVolumePass::renderAABB(const AABB& aabb, FXMMATRIX object_to_world) const {
	
	const auto scale  = aabb.max() - aabb.min();
	const auto center = (aabb.max() + aabb.min()) * 0.5f;

	auto box_to_object  = XMMatrixRotationRollPitchYawFromVector(g_XMZero);
	box_to_object.r[0] *= XMVectorGetX(scale);
	box_to_object.r[1] *= XMVectorGetY(scale);
	box_to_object.r[2] *= XMVectorGetZ(scale);
	box_to_object.r[3]  = XMVectorSetW(center, 1.0f);

	const auto box_to_world = box_to_object * object_to_world;
	model_matrix_buffer.updateData(device_context, XMMatrixTranspose(box_to_world));

	Pipeline::draw(device_context, 24, 0);
}