#include "bounding_volume_pass.h"

#include "hlsl.h"
#include "scene/scene.h"
#include "render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader_factory.h"
#include "geometry/frustum/frustum.h"


BoundingVolumePass::BoundingVolumePass(ID3D11Device& device,
									   ID3D11DeviceContext& device_context,
									   RenderStateMgr& render_state_mgr,
									   ResourceMgr& resource_mgr)
	: device_context(device_context)
	, render_state_mgr(render_state_mgr)
	, model_matrix_buffer(device) {

	vertex_shader = ShaderFactory::createWireframeBoxVS(resource_mgr);
	pixel_shader  = ShaderFactory::createWireframeBoxPS(resource_mgr);
}


void BoundingVolumePass::bindRenderStates() const {

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
	render_state_mgr.get().bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.get().bind(device_context, BlendStates::Opaque);
	render_state_mgr.get().bind(device_context, RasterStates::CullCounterClockwise);
}


void XM_CALLCONV BoundingVolumePass::render(Scene& scene, FXMMATRIX world_to_projection) const {
	
	auto& ecs_engine = scene.getECS();

	// Bind the render states
	bindRenderStates();

	ecs_engine.forEach<DirectionalLight>([&](const DirectionalLight& light) {

		if (!light.isActive()) return;
		
		const auto transform = ecs_engine.getEntity(light.getOwner())->getComponent<Transform>();
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getAABB()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<PointLight>([&](const PointLight& light) {

		if (!light.isActive()) return;

		const auto transform = ecs_engine.getEntity(light.getOwner())->getComponent<Transform>();
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getBoundingSphere()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<SpotLight>([&](const SpotLight& light) {

		if (!light.isActive()) return;

		const auto transform = ecs_engine.getEntity(light.getOwner())->getComponent<Transform>();
		if (!transform) return;

		const auto object_to_world      = transform->getObjectToWorldMatrix();
		const auto object_to_projection = object_to_world * world_to_projection;

		if (!Frustum(object_to_projection).contains(light.getAABB()))
			return;

		renderAABB(light.getAABB(), object_to_world);
	});

	ecs_engine.forEach<Model>([&](const Model& model) {

		if (!model.isActive()) return;

		const auto transform = ecs_engine.getEntity(model.getOwner())->getComponent<Transform>();
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