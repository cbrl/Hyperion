#include "depth_pass.h"
#include "engine/engine.h"
#include "pipeline.h"
#include "hlsl.h"

#include "directx/vertex_types.h"
#include "geometry/frustum/frustum.h"

//#include "compiled_headers/depth.h"
#include "compiled_headers/depth_vs.h"


DepthPass::DepthPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
	, alt_cam_buffer(device) {

	vertex_shader = make_unique<VertexShader>(device,
	                                          shader_depth_vs,
	                                          sizeof(shader_depth_vs),
	                                          VertexPositionNormalTexture::InputElements,
	                                          VertexPositionNormalTexture::InputElementCount);

	// Pixel shader for transparent objects
	//pixel_shader = make_unique<PixelShader>(device, "FILENAME");
}


void DepthPass::bindState(const RenderStateMgr& render_state_mgr) const {
	// Bind null shaders
	Pipeline::HS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::DS::bindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::bindShader(device_context, nullptr, nullptr, 0);

	// Raster state
	render_state_mgr.bindCullCounterClockwise(device_context);

	// Depth state
	render_state_mgr.bindDepthDefault(device_context);
}


void XM_CALLCONV DepthPass::render(const Engine& engine,
                                   FXMMATRIX world_to_camera,
                                   CXMMATRIX camera_to_projection) const {

	auto& ecs_engine = engine.getECS();

	// Update and bind the camera buffer
	updateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;

	// Bind the vertex shader
	vertex_shader->bind(device_context);
	Pipeline::PS::bindShader(device_context, nullptr, nullptr, 0);

	// Draw each model
	ecs_engine.forEach<Model>([&](const Model& model) {

		if (!model.isActive()) return;

		const auto transform = ecs_engine.getComponent<Transform>(model.getOwner());
		if (!transform) return;

		const auto model_to_world = transform->getObjectToWorldMatrix();
		const auto model_to_proj  = model_to_world * world_to_proj;

		if (!Frustum(model_to_proj).contains(model.getAABB()))
			return;

		model.bind(device_context);

		renderModel(model, model_to_proj);
	});

}

void XM_CALLCONV DepthPass::renderShadows(const Engine& engine,
                                          FXMMATRIX world_to_camera,
                                          CXMMATRIX camera_to_projection) const {

	auto& ecs_engine = engine.getECS();

	// Update and bind the camera buffer
	updateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;

	// Bind the vertex shader and a null pixel shader
	vertex_shader->bind(device_context);
	Pipeline::PS::bindShader(device_context, nullptr, nullptr, 0);

	// Draw each model
	ecs_engine.forEach<Model>([&](Model& model) {

		if (!model.isActive()) return;

		const auto transform = ecs_engine.getComponent<Transform>(model.getOwner());
		if (!transform) return;

		const auto model_to_world = transform->getObjectToWorldMatrix();
		const auto model_to_proj  = model_to_world * world_to_proj;

		if (!Frustum(model_to_proj).contains(model.getAABB()))
			return;

		model.bind(device_context);

		renderModel(model, model_to_proj);
	});
}


void XM_CALLCONV DepthPass::updateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) const {

	AltCameraBuffer buffer;
	buffer.world_to_camera      = XMMatrixTranspose(world_to_camera);
	buffer.camera_to_projection = XMMatrixTranspose(camera_to_projection);

	alt_cam_buffer.updateData(device_context, buffer);
	alt_cam_buffer.bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA_ALT);
}


void XM_CALLCONV DepthPass::renderModel(const Model& model, FXMMATRIX model_to_projection) const {

	model.forEachChild([&](const ModelChild& child) {
		//if (child.GetMaterial().transparent
		//	|| child.GetMaterial().dissolve > THRESHOLD)
		//	return;

		if (!child.castsShadows())
			return;

		if (!Frustum(model_to_projection).contains(child.getAABB()))
			return;

		child.bindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

		Pipeline::drawIndexed(device_context, child.getIndexCount(), child.getIndexStart());
	});
}
