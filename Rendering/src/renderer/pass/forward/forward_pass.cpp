module;

#include "memory/handle/handle.h"

#include "hlsl.h"
#include "directx/d3d11.h"
#include "directxmath/directxmath.h"

module rendering:pass.forward_pass;

import ecs;
import components.model;
import components.transform;

import math.geometry;

import :pipeline;
import :rendering_options;
import :render_state_mgr;
import :resource_mgr;
import :shader_factory;


namespace render {

ForwardPass::ForwardPass(ID3D11Device& device,
                         ID3D11DeviceContext& device_context,
                         RenderStateMgr& render_state_mgr,
                         ResourceMgr& resource_mgr)
    : device_context(device_context)
    , render_state_mgr(render_state_mgr)
    , resource_mgr(resource_mgr)
    , color_buffer(device) {

	vertex_shader  = ShaderFactory::CreateForwardVS(resource_mgr);
	gbuffer_shader = ShaderFactory::CreateGBufferPS(resource_mgr);
}


void ForwardPass::bindOpaqueState() const {

	// Bind topology
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Unbind shaders
	Pipeline::DS::bindShader(device_context, nullptr, {});
	Pipeline::GS::bindShader(device_context, nullptr, {});
	Pipeline::HS::bindShader(device_context, nullptr, {});

	// Bind shaders
	vertex_shader->bind(device_context);

	// Bind render states
	render_state_mgr.bind(device_context, BlendStates::Opaque);
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);
}


void ForwardPass::bindTransparentState() const {

	// Bind topology
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind null shaders
	Pipeline::DS::bindShader(device_context, nullptr, {});
	Pipeline::GS::bindShader(device_context, nullptr, {});
	Pipeline::HS::bindShader(device_context, nullptr, {});

	// Bind shaders
	vertex_shader->bind(device_context);

	// Bind render states
	render_state_mgr.bind(device_context, BlendStates::NonPremultiplied);
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);
}


void ForwardPass::bindWireframeState() const {

	// Bind topology
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind null shaders
	Pipeline::DS::bindShader(device_context, nullptr, {});
	Pipeline::GS::bindShader(device_context, nullptr, {});
	Pipeline::HS::bindShader(device_context, nullptr, {});

	color_buffer.bind<Pipeline::PS>(device_context, SLOT_CBUFFER_COLOR);

	// Bind shaders
	vertex_shader->bind(device_context);

	// Bind render states
	render_state_mgr.bind(device_context, BlendStates::Opaque);
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.bind(device_context, RasterStates::Wireframe);
}


void XM_CALLCONV ForwardPass::renderOpaque(const ecs::ECS& ecs,
                                           FXMMATRIX world_to_projection,
                                           const Texture* env_map,
                                           BRDF brdf) const {

	// Bind the vertex shader, render states, etc
	bindOpaqueState();

	// Bind the skybox texture as the environment map
	if (env_map) env_map->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Create the apporopriate pixel shader and bind it
	auto pixel_shader = ShaderFactory::CreateForwardPS(resource_mgr, brdf, false);
	pixel_shader->bind(device_context);

	// Render models
	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		if (not model.isActive())
			return;

		const auto& mat = model.getMaterial();
		if (mat.shader || mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, transform, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderTransparent(const ecs::ECS& ecs,
                                                FXMMATRIX world_to_projection,
                                                const Texture* env_map,
                                                BRDF brdf) const {

	// Bind the vertex shader, render states, etc
	bindTransparentState();

	// Bind the skybox texture as the environment map
	if (env_map) env_map->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Create the apporopriate pixel shader and bind it
	auto pixel_shader = ShaderFactory::CreateForwardPS(resource_mgr, brdf, true);
	pixel_shader->bind(device_context);

	// Render models
	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		if (not model.isActive())
			return;

		const auto& mat = model.getMaterial();
		if (mat.shader ||
		    mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
			return;

		renderModel(model, transform, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderOverrided(const ecs::ECS& ecs, FXMMATRIX world_to_projection, const Texture* env_map) const {

	//----------------------------------------------------------------------------------
	// Sort models by shader type
	//----------------------------------------------------------------------------------
	std::unordered_map<PixelShader*, std::vector<const Model*>> sorted_models;

	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model = ecs.get<Model>(entity);
		auto& mat = model.getMaterial();

		if (not model.isActive())
			return;
		if (not mat.shader)
			return;

		sorted_models[mat.shader.get()].push_back(&model);
	});


	// Bind the environment map
	if (env_map) {
		env_map->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);
	}


	//----------------------------------------------------------------------------------
	// Render opaque models
	//----------------------------------------------------------------------------------
	bindOpaqueState();

	for (auto& [shader, model_vec] : sorted_models) {
		shader->bind(device_context);

		for (const auto* model : model_vec) {
			const auto& transform = ecs.get<Transform>(model->getOwner());
			const auto& mat = model->getMaterial();

			if (mat.params.base_color[3] <= ALPHA_MAX)
				continue;

			renderModel(*model, transform, world_to_projection);
		}
	}

	//----------------------------------------------------------------------------------
	// Render transparent models
	//----------------------------------------------------------------------------------
	bindTransparentState();

	for (auto& [shader, model_vec] : sorted_models) {
		shader->bind(device_context);

		for (const auto* model : model_vec) {
			const auto& transform = ecs.get<Transform>(model->getOwner());
			const auto& mat = model->getMaterial();

			if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
				continue;

			renderModel(*model, transform, world_to_projection);
		}
	}
}


void ForwardPass::renderFalseColor(const ecs::ECS& ecs,
                                   FXMMATRIX world_to_projection,
                                   FalseColor color) const {

	bindOpaqueState();

	auto pixel_shader = ShaderFactory::CreateFalseColorPS(resource_mgr, color);
	pixel_shader->bind(device_context);

	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		if (model.isActive())
			renderModel(model, transform, world_to_projection);
	});
}


void ForwardPass::renderWireframe(const ecs::ECS& ecs, FXMMATRIX world_to_projection, const f32_4& color) const {

	bindWireframeState();

	color_buffer.updateData(device_context, color);

	auto pixel_shader = ShaderFactory::CreateFalseColorPS(resource_mgr, FalseColor::Static);
	pixel_shader->bind(device_context);

	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		if (model.isActive())
			renderModel(model, transform, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderGBuffer(const ecs::ECS& ecs, FXMMATRIX world_to_projection) const {

	bindOpaqueState();

	gbuffer_shader->bind(device_context);

	ecs.forEach<Model, Transform>([&](handle64 entity) {
		const auto& model     = ecs.get<Model>(entity);
		const auto& transform = ecs.get<Transform>(entity);

		if (not model.isActive())
			return;

		const auto& mat = model.getMaterial();
		if (mat.shader || mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, transform, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderModel(const Model& model, const Transform& transform, FXMMATRIX world_to_projection) const {
	const auto model_to_world = transform.getObjectToWorldMatrix();
	const auto model_to_proj = model_to_world * world_to_projection;

	// Cull the model if it isn't on screen
	if (not Frustum(model_to_proj).contains(model.getAABB()))
		return;

	// Bind the model's mesh
	model.bindMesh(device_context);

	// Get the model's material
	const auto& mat = model.getMaterial();

	// Bind the model's buffer
	model.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
	model.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);

	// Bind the SRVs
	if (mat.maps.base_color) mat.maps.base_color->bind<Pipeline::PS>(device_context, SLOT_SRV_BASE_COLOR);
	if (mat.maps.material_params) mat.maps.material_params->bind<Pipeline::PS>(device_context, SLOT_SRV_MATERIAL_PARAMS);
	if (mat.maps.normal) mat.maps.normal->bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);
	if (mat.maps.emissive) mat.maps.emissive->bind<Pipeline::PS>(device_context, SLOT_SRV_EMISSIVE);

	// Draw the model
	Pipeline::drawIndexed(device_context, model.getIndexCount(), 0);

	// Unbind the SRVs
	Pipeline::PS::bindSRV(device_context, SLOT_SRV_BASE_COLOR, nullptr);
	Pipeline::PS::bindSRV(device_context, SLOT_SRV_MATERIAL_PARAMS, nullptr);
	Pipeline::PS::bindSRV(device_context, SLOT_SRV_NORMAL, nullptr);
	Pipeline::PS::bindSRV(device_context, SLOT_SRV_EMISSIVE, nullptr);
}

} //namespace render