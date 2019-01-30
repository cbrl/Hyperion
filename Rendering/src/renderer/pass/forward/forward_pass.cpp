#include "forward_pass.h"

#include "hlsl.h"
#include "scene/scene.h"
#include "renderer/state/render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "geometry/frustum/frustum.h"


ForwardPass::ForwardPass(ID3D11Device& device,
                         ID3D11DeviceContext& device_context,
                         RenderStateMgr& render_state_mgr,
                         ResourceMgr& resource_mgr)
	: device_context(device_context)
	, render_state_mgr(render_state_mgr)
	, resource_mgr(resource_mgr)
	, color_buffer(device) {

	vertex_shader = ShaderFactory::CreateForwardVS(resource_mgr);
}


void ForwardPass::bindOpaqueState() const {
	
	// Bind topology
	Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind null shaders
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

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
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

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
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);

	color_buffer.bind<Pipeline::PS>(device_context, SLOT_CBUFFER_COLOR);

	// Bind shaders
	vertex_shader->bind(device_context);

	// Bind render states
	render_state_mgr.bind(device_context, BlendStates::Opaque);
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
	render_state_mgr.bind(device_context, RasterStates::Wireframe);
}


void XM_CALLCONV ForwardPass::renderOpaque(const std::vector<std::reference_wrapper<const Model>>& models,
                                           FXMMATRIX world_to_projection,
                                           const Texture* env_map,
                                           PixelShader* shader) const {

	// Bind the vertex shader, render states, etc
	bindOpaqueState();

	// Bind the skybox texture as the environment map
	if (env_map) env_map->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Default pixel shader. Used if shader == nullptr
	static auto default_shader = ShaderFactory::CreateForwardPS(resource_mgr, BRDF::CookTorrance, false);

	// Bind the shader
	if (shader)
		shader->bind(device_context);
	else
		default_shader->bind(device_context);

	// Render each model
	for (auto& model : models) {
		auto& mat = model.get().getMaterial();
		if (mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, world_to_projection);
	}
}


void XM_CALLCONV ForwardPass::renderTransparent(const std::vector<std::reference_wrapper<const Model>>& models,
                                                FXMMATRIX world_to_projection,
                                                const Texture* env_map,
                                                PixelShader* shader) const {

	// Bind the vertex shader, render states, etc
	bindTransparentState();

	// Bind the skybox texture as the environment map
	if (env_map) env_map->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Default pixel shader. Used if shader == nullptr
	auto default_shader = ShaderFactory::CreateForwardPS(resource_mgr, BRDF::CookTorrance, true);

	// Bind the shader
	if (shader)
		shader->bind(device_context);
	else
		default_shader->bind(device_context);

	// Render each model
	for (auto& model : models) {
		auto& mat = model.get().getMaterial();
		if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
			return;

		renderModel(model, world_to_projection);
	}
}


void XM_CALLCONV ForwardPass::renderOpaque(Scene& scene,
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
	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;

		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderTransparent(Scene& scene,
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
	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;

		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
			return;

		renderModel(model, world_to_projection);
	});
}


void ForwardPass::renderFalseColor(Scene& scene,
                                   FXMMATRIX world_to_projection,
                                   FalseColor color) const {

	bindOpaqueState();

	auto pixel_shader = ShaderFactory::CreateFalseColorPS(resource_mgr, color);
	pixel_shader->bind(device_context);

	scene.forEach<Model>([&](const Model& model) {
		if (model.isActive())
			renderModel(model, world_to_projection);
	});
}


void ForwardPass::renderWireframe(Scene& scene, FXMMATRIX world_to_projection, const vec4_f32& color) const {

	bindWireframeState();

	color_buffer.updateData(device_context, color);

	auto pixel_shader = ShaderFactory::CreateFalseColorPS(resource_mgr, FalseColor::Static);
	pixel_shader->bind(device_context);

	scene.forEach<Model>([&](const Model& model) {
		if (model.isActive())
			renderModel(model, world_to_projection);
	});
}


void XM_CALLCONV ForwardPass::renderModel(const Model& model, FXMMATRIX world_to_projection) const {

	const auto* transform = model.getOwner()->getComponent<Transform>();
	if (!transform) return;

	const auto model_to_world = transform->getObjectToWorldMatrix();
	const auto model_to_proj  = model_to_world * world_to_projection;

	// Cull the model if it isn't on screen
	if (!Frustum(model_to_proj).contains(model.getAABB()))
		return;

	// Bind the model's mesh
	model.bindMesh(device_context);

	// Get the model's material
	const auto& mat = model.getMaterial();

	// Bind the model's buffer
	model.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);
	model.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);

	// Bind the SRVs
	if (mat.maps.base_color)      mat.maps.base_color->bind<Pipeline::PS>(device_context, SLOT_SRV_BASE_COLOR);
	if (mat.maps.material_params) mat.maps.material_params->bind<Pipeline::PS>(device_context, SLOT_SRV_MATERIAL_PARAMS);
	if (mat.maps.normal)          mat.maps.normal->bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);
	if (mat.maps.emissive)        mat.maps.emissive->bind<Pipeline::PS>(device_context, SLOT_SRV_EMISSIVE);


	// Draw the model
	Pipeline::drawIndexed(device_context, model.getIndexCount(), 0);


	// Unbind the SRVs
	// Slot definition could be used as a more dynamic way of unbinding any amount of srvs
	// E.g. null_srv[SLOT_SRV_EMISSIVE + 1] = { nullptr };
	ID3D11ShaderResourceView* null_srv[4] = { nullptr };
	Pipeline::PS::bindSRVs(device_context, 0, 4, null_srv);
}
