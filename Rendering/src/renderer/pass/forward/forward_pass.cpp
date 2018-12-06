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

	vertex_shader = ShaderFactory::createForwardVS(resource_mgr);
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


void XM_CALLCONV ForwardPass::renderOpaque(Scene& scene,
                                           FXMMATRIX world_to_projection,
                                           const Texture* sky) const {

	// Bind the shaders, render states, etc
	bindOpaqueState();

	// Bind the skybox texture as the environment map
	if (sky) sky->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Create the apporopriate pixel shader and bind it
	const auto pixel_shader = ShaderFactory::createForwardPS(resource_mgr, false);
	pixel_shader->bind(device_context);

	// Render models
	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		if (!root.isActive()) return;

		root.forEachModel([&](const Model& model) {
			//TODO: if (!model.isActive()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.diffuse.w <= ALPHA_MAX)
				return;

			renderModel(root, model, world_to_projection);
		});
	});
}


void XM_CALLCONV ForwardPass::renderTransparent(Scene& scene,
                                                FXMMATRIX world_to_projection,
                                                const Texture* sky) const {

	// Bind the shaders, render states, etc
	bindTransparentState();

	// Bind the skybox texture as the environment map
	if (sky) sky->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);

	// Create the apporopriate pixel shader and bind it
	const auto pixel_shader = ShaderFactory::createForwardPS(resource_mgr, true);
	pixel_shader->bind(device_context);

	// Render models
	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		if (!root.isActive()) return;

		root.forEachModel([&](const Model& model) {
			//TODO: if (!model.isActive()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.opacity < ALPHA_MIN || mat.params.opacity > ALPHA_MAX)
				return;

			renderModel(root, model, world_to_projection);
		});
	});
}


void XM_CALLCONV ForwardPass::renderUnlit(Scene& scene,
                                          FXMMATRIX world_to_projection,
                                          const Texture* sky) const {

	// Bind the skybox texture as the environment map
	if (sky) sky->bind<Pipeline::PS>(device_context, SLOT_SRV_ENV_MAP);


	// Opaque models
	bindTransparentState();
	const auto opaque_ps = ShaderFactory::createForwardUnlitPS(resource_mgr, false);
	opaque_ps->bind(device_context);

	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		if (!root.isActive()) return;

		root.forEachModel([&](const Model& model) {
			//TODO: if (!model.isActive()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.opacity <= ALPHA_MAX)
				return;

			renderModel(root, model, world_to_projection);
		});
	});


	// Transparent Models
	bindTransparentState();
	const auto transparent_ps = ShaderFactory::createForwardUnlitPS(resource_mgr, true);
	transparent_ps->bind(device_context);

	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		if (!root.isActive()) return;

		root.forEachModel([&](const Model& model) {
			//TODO: if (!model.isActive()) return;

			const auto& mat = model.getMaterial();
			if (mat.params.opacity< ALPHA_MIN || mat.params.opacity > ALPHA_MAX)
				return;

			renderModel(root, model, world_to_projection);
		});
	});
}


void ForwardPass::renderFalseColor(Scene& scene,
                                   FXMMATRIX world_to_projection,
                                   FalseColor color) const {

	bindOpaqueState();

	const auto pixel_shader = ShaderFactory::createFalseColorPS(resource_mgr, color);
	pixel_shader->bind(device_context);

	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		if (!root.isActive()) return;

		root.forEachModel([&](const Model& model) {
			/* TODO:
			if (model.isActive())
				renderModel(model, world_to_projection);
			*/
		});
	});
}


void ForwardPass::renderWireframe(Scene& scene, FXMMATRIX world_to_projection, const vec4_f32& color) const {

	bindWireframeState();

	color_buffer.updateData(device_context, color);

	const auto pixel_shader = ShaderFactory::createFalseColorPS(resource_mgr, FalseColor::Static);
	pixel_shader->bind(device_context);

	scene.forEach<ModelRoot>([&](const ModelRoot& root) {
		root.forEachModel([&](const Model& model) {
			/* TODO:
			if (model.isActive())
				renderModel(model, world_to_projection);
			*/
		});
	});
}


void XM_CALLCONV ForwardPass::renderModel(const ModelRoot& root, const Model& model, FXMMATRIX world_to_projection) const {

	// TODO: More elegant way of passing transform/root to function
	const auto* transform = root.getOwner()->getComponent<Transform>();
	if (!transform) return;

	const auto model_to_world = transform->getObjectToWorldMatrix();
	const auto model_to_proj  = model_to_world * world_to_projection;

	// Cull the model if it isn't on screen
	Frustum frustum(model_to_proj);
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
	if (mat.maps.diffuse)       mat.maps.diffuse->bind<Pipeline::PS>(device_context, SLOT_SRV_DIFFUSE);
	if (mat.maps.ambient)       mat.maps.ambient->bind<Pipeline::PS>(device_context, SLOT_SRV_AMBIENT);
	if (mat.maps.normal)        mat.maps.normal->bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);
	if (mat.maps.specular)      mat.maps.specular->bind<Pipeline::PS>(device_context, SLOT_SRV_SPECULAR);
	if (mat.maps.spec_exponent) mat.maps.spec_exponent->bind<Pipeline::PS>(device_context, SLOT_SRV_SPEC_EXPONENT);
	if (mat.maps.opacity)       mat.maps.opacity->bind<Pipeline::PS>(device_context, SLOT_SRV_ALPHA);
	if (mat.maps.height)        mat.maps.height->bind<Pipeline::PS>(device_context, SLOT_SRV_NORMAL);


	// Draw the model
	Pipeline::drawIndexed(device_context, model.getIndexCount(), 0);


	// Unbind the SRVs
	// Slot definition could be used as a more dynamic way of unbinding any amount of srvs
	// E.g. null_srv[SLOT_SRV_ALPHA + 1] = { nullptr };
	ID3D11ShaderResourceView* null_srv[6] = { nullptr };
	Pipeline::PS::bindSRVs(device_context, 0, 6, null_srv);
}
