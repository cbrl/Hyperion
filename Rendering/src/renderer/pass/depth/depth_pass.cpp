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

	opaque_vs      = ShaderFactory::createDepthVS(resource_mgr);
	transparent_vs = ShaderFactory::createDepthTransparentVS(resource_mgr);
	transparent_ps = ShaderFactory::createDepthTransparentPS(resource_mgr);
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

	auto& ecs_engine = scene.getECS();

	// Update and bind the camera buffer
	updateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;


	//----------------------------------------------------------------------------------
	// Draw each opaque model
	//----------------------------------------------------------------------------------

	ecs_engine.forEach<Model>([&](Model& model) {
		const auto& mat = model.getMaterial();
		if (mat.transparent && mat.diffuse.w <= ALPHA_MAX)
			return;

		renderModel(ecs_engine, model, world_to_proj);
	});


	//----------------------------------------------------------------------------------
	// Draw each transparent model
	//----------------------------------------------------------------------------------

	ecs_engine.forEach<Model>([&](Model& model) {
		const auto& mat = model.getMaterial();
		if (!mat.transparent
			|| mat.diffuse.w < ALPHA_MIN
			|| mat.diffuse.w > ALPHA_MAX)
			return;

		renderModel(ecs_engine, model, world_to_proj);
	});
}

void XM_CALLCONV DepthPass::renderShadows(Scene& scene,
                                          FXMMATRIX world_to_camera,
                                          CXMMATRIX camera_to_projection) const {

	auto& ecs_engine = scene.getECS();

	updateCamera(world_to_camera, camera_to_projection);

	const auto world_to_proj = world_to_camera * camera_to_projection;

	//----------------------------------------------------------------------------------
	// Draw each opaque model
	//----------------------------------------------------------------------------------

	bindOpaqueShaders();
	ecs_engine.forEach<Model>([&](Model& model) {
		if (!model.castsShadows()) return;

		const auto& mat = model.getMaterial();
		if (mat.transparent && mat.diffuse.w <= ALPHA_MAX)
			return;

		renderModel(ecs_engine, model, world_to_proj);
	});


	//----------------------------------------------------------------------------------
	// Draw each transparent model
	//----------------------------------------------------------------------------------

	bindTransparentShaders();
	ecs_engine.forEach<Model>([&](Model& model) {
		if (!model.castsShadows()) return;

		const auto& mat = model.getMaterial();
		if (!mat.transparent
			|| mat.diffuse.w < ALPHA_MIN
			|| mat.diffuse.w > ALPHA_MAX)
			return;

		renderModel(ecs_engine, model, world_to_proj);
	});
}


void XM_CALLCONV DepthPass::updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const {

	AltCameraBuffer buffer;
	buffer.world_to_camera      = XMMatrixTranspose(world_to_camera);
	buffer.camera_to_projection = XMMatrixTranspose(camera_to_projection);

	alt_cam_buffer.updateData(device_context, buffer);
	alt_cam_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA_ALT);
}


void XM_CALLCONV DepthPass::renderModel(ECS& ecs_engine,
                                        const Model& model,
                                        FXMMATRIX world_to_projection) const {

	if (!model.isActive()) return;

	const auto transform = model.getOwner()->getComponent<Transform>();
	if (!transform) return;

	const auto model_to_world      = transform->getObjectToWorldMatrix();
	const auto model_to_projection = model_to_world * world_to_projection;

	Frustum frustum{model_to_projection};
	if (!frustum.contains(model.getAABB()))
		return;

	model.bind(device_context);
	model.bindBuffer<Pipeline::PS>(device_context, SLOT_CBUFFER_MODEL);
	model.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

	const auto& mat = model.getMaterial();
	if (mat.map_diffuse)
		mat.map_diffuse->bind<Pipeline::PS>(device_context, SLOT_SRV_DIFFUSE);

	Pipeline::drawIndexed(device_context, model.getIndexCount(), model.getIndexStart());

	Pipeline::PS::bindSRV(device_context, SLOT_SRV_DIFFUSE, nullptr);
}
