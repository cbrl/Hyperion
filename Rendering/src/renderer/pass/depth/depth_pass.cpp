#include "depth_pass.h"

#include "hlsl.h"
#include "scene/scene.h"
#include "renderer/state/render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader_factory.h"
#include "geometry/frustum/frustum.h"


DepthPass::DepthPass(ID3D11Device& device,
                     ID3D11DeviceContext& device_context,
                     RenderStateMgr& render_state_mgr,
                     ResourceMgr& resource_mgr)
	: device_context(device_context)
	, render_state_mgr(render_state_mgr)
	, alt_cam_buffer(device) {

	opaque_vs      = ShaderFactory::CreateDepthVS(resource_mgr);
	transparent_vs = ShaderFactory::CreateDepthTransparentVS(resource_mgr);
	transparent_ps = ShaderFactory::CreateDepthTransparentPS(resource_mgr);
}


void DepthPass::bindState() const {

	// Bind null shaders
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);

	// Render States
	render_state_mgr.bind(device_context, RasterStates::CullCounterClockwise);
	render_state_mgr.bind(device_context, DepthStencilStates::LessEqRW);
}


void DepthPass::bindOpaqueShaders() const {
	opaque_vs->bind(device_context);
	Pipeline::PS::bindShader(device_context, nullptr, nullptr, 0);
}


void DepthPass::bindTransparentShaders() const {
	transparent_vs->bind(device_context);
	transparent_ps->bind(device_context);
}


void XM_CALLCONV DepthPass::render(Scene& scene,
                                   FXMMATRIX world_to_camera,
                                   CXMMATRIX camera_to_projection) const {

	// Update and bind the camera buffer
	updateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;


	//----------------------------------------------------------------------------------
	// Draw each opaque model
	//----------------------------------------------------------------------------------

	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;

		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, world_to_proj);
	});


	//----------------------------------------------------------------------------------
	// Draw each transparent model
	//----------------------------------------------------------------------------------

	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;
		
		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
			return;

		renderModel(model, world_to_proj);
	});
}

void XM_CALLCONV DepthPass::renderShadows(Scene& scene,
                                          FXMMATRIX world_to_camera,
                                          CXMMATRIX camera_to_projection) const {

	updateCamera(world_to_camera, camera_to_projection);

	const auto world_to_proj = world_to_camera * camera_to_projection;

	//----------------------------------------------------------------------------------
	// Draw each opaque model
	//----------------------------------------------------------------------------------

	bindOpaqueShaders();
	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;
		if (!model.castsShadows()) return;

		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] <= ALPHA_MAX)
			return;

		renderModel(model, world_to_proj);
	});


	//----------------------------------------------------------------------------------
	// Draw each transparent model
	//----------------------------------------------------------------------------------

	bindTransparentShaders();
	scene.forEach<Model>([&](const Model& model) {
		if (!model.isActive()) return;
		if (!model.castsShadows()) return;

		const auto& mat = model.getMaterial();
		if (mat.params.base_color[3] < ALPHA_MIN || mat.params.base_color[3] > ALPHA_MAX)
			return;

		renderModel(model, world_to_proj);
	});
}


void XM_CALLCONV DepthPass::updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const {

	AltCameraBuffer buffer;
	buffer.world_to_camera      = XMMatrixTranspose(world_to_camera);
	buffer.camera_to_projection = XMMatrixTranspose(camera_to_projection);

	alt_cam_buffer.updateData(device_context, buffer);
	alt_cam_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA_ALT);
}


void XM_CALLCONV DepthPass::renderModel(const Model& model, FXMMATRIX world_to_projection) const {

	const auto* transform = model.getOwner()->getComponent<Transform>();
	if (!transform) return;

	const auto model_to_world      = transform->getObjectToWorldMatrix();
	const auto model_to_projection = model_to_world * world_to_projection;

	if (!Frustum(model_to_projection).contains(model.getAABB()))
		return;

	model.bindMesh(device_context);
	model.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);
	model.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

	const auto& mat = model.getMaterial();
	if (mat.maps.base_color)
		mat.maps.base_color->bind<Pipeline::PS>(device_context, SLOT_SRV_BASE_COLOR);

	Pipeline::drawIndexed(device_context, model.getIndexCount(), 0);

	Pipeline::PS::bindSRV(device_context, SLOT_SRV_BASE_COLOR, nullptr);
}
