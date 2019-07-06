#include "bounding_volume_pass.h"

#include "hlsl.h"
#include "scene/scene.h"
#include "renderer/state/render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader_factory.h"
#include "geometry/frustum/frustum.h"


namespace render {

BoundingVolumePass::BoundingVolumePass(ID3D11Device& device,
									   ID3D11DeviceContext& device_context,
									   RenderStateMgr& render_state_mgr,
									   ResourceMgr& resource_mgr)
	: device_context(device_context)
	, render_state_mgr(render_state_mgr)
	, model_matrix_buffer(device)
	, color_buffer(device) {

	vertex_shader = ShaderFactory::CreateWireframeBoxVS(resource_mgr);
	pixel_shader  = ShaderFactory::CreateWireframeBoxPS(resource_mgr);
}


void BoundingVolumePass::bindRenderStates() const {

	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Unbind shaders
	Pipeline::DS::bindShader(device_context, nullptr, {});
	Pipeline::GS::bindShader(device_context, nullptr, {});
	Pipeline::HS::bindShader(device_context, nullptr, {});

	// Bind the vertex and pixel shaders
	vertex_shader->bind(device_context);
	pixel_shader->bind(device_context);

	// Bind the constant buffers
	model_matrix_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
	color_buffer.bind<Pipeline::PS>(device_context, SLOT_CBUFFER_COLOR);

	// Bind the render states
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.bind(device_context, BlendStates::Opaque);
	render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);
}


void XM_CALLCONV BoundingVolumePass::render(const ecs::ECS& ecs, FXMMATRIX world_to_projection, const f32_4& color) const {

	// Bind the render states
	bindRenderStates();

	color_buffer.updateData(device_context, color);

	ecs.forEach<Transform, DirectionalLight>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& light     = ecs.get<DirectionalLight>(entity);

		if (not light.isActive())
			return;

		renderAABB(light.getAABB(), transform, world_to_projection);
	});

	ecs.forEach<Transform, PointLight>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& light     = ecs.get<PointLight>(entity);

		if (not light.isActive())
			return;

		renderAABB(light.getAABB(), transform, world_to_projection);
	});

	ecs.forEach<Transform, SpotLight>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& light     = ecs.get<SpotLight>(entity);

		if (not light.isActive())
			return;

		renderAABB(light.getAABB(), transform, world_to_projection);
	});

	ecs.forEach<Transform, Model>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& model     = ecs.get<Model>(entity);

		if (not model.isActive())
			return;

		renderAABB(model.getAABB(), transform, world_to_projection);
	});
}


void XM_CALLCONV BoundingVolumePass::renderAABB(const AABB& aabb, const Transform& transform, FXMMATRIX world_to_projection) const {

	const auto object_to_world = transform.getObjectToWorldMatrix();
	const auto object_to_projection = object_to_world * world_to_projection;

	if (not Frustum(object_to_projection).contains(aabb))
		return;
	
	const auto scale  = aabb.max() - aabb.min();
	const auto center = (aabb.max() + aabb.min()) * 0.5f;

	auto box_to_object  = XMMatrixIdentity();
	box_to_object.r[0] *= XMVectorGetX(scale);
	box_to_object.r[1] *= XMVectorGetY(scale);
	box_to_object.r[2] *= XMVectorGetZ(scale);
	box_to_object.r[3]  = XMVectorSetW(center, 1.0f);

	const auto box_to_world = box_to_object * object_to_world;
	model_matrix_buffer.updateData(device_context, XMMatrixTranspose(box_to_world));

	Pipeline::draw(device_context, 24, 0);
}

} //namespace render