#include "stdafx.h"
#include "depth_pass.h"
#include "engine\engine.h"

//#include "compiled_headers\depth.h"
#include "compiled_headers\depth_vs.h"


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


void DepthPass::BindState(const RenderStateMgr& render_state_mgr) const {
	// Bind null shaders
	Pipeline::HS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::DS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::BindShader(device_context, nullptr, nullptr, 0);

	// Raster state
	render_state_mgr.BindCullCounterClockwise(device_context);

	// Depth state
	render_state_mgr.BindDepthDefault(device_context);
}


void XM_CALLCONV DepthPass::Render(const Engine& engine,
								   FXMMATRIX world_to_camera,
								   CXMMATRIX camera_to_projection) {

	auto& ecs_engine = engine.GetECS();

	// Update and bind the camera buffer
	UpdateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;

	// Bind the vertex shader
	vertex_shader->Bind(device_context);
	Pipeline::PS::BindShader(device_context, nullptr, nullptr, 0);

	// Draw each model
	ecs_engine.forEachActive<Model>([&](Model& model) {
		const auto transform      = ecs_engine.getComponent<Transform>(model.getOwner());
		const auto model_to_world = transform->GetObjectToWorldMatrix();
		const auto model_to_proj  = model_to_world * world_to_proj;

		if (!Frustum(model_to_proj).Contains(model.GetAABB()))
			return;

		model.Bind(device_context);

		RenderModel(model, model_to_proj);
	});

}

void XM_CALLCONV DepthPass::RenderShadows(const Engine& engine,
										  FXMMATRIX world_to_camera,
										  CXMMATRIX camera_to_projection) {

	auto& ecs_engine = engine.GetECS();

	// Update and bind the camera buffer
	UpdateCamera(world_to_camera, camera_to_projection);

	// Create the world_to_projection matrix
	const auto world_to_proj = world_to_camera * camera_to_projection;

	// Bind the vertex shader and a null pixel shader
	vertex_shader->Bind(device_context);
	Pipeline::PS::BindShader(device_context, nullptr, nullptr, 0);

	// Draw each model
	ecs_engine.forEachActive<Model>([&](Model& model) {
		const auto transform      = ecs_engine.getComponent<Transform>(model.getOwner());
		const auto model_to_world = transform->GetObjectToWorldMatrix();
		const auto model_to_proj  = model_to_world * world_to_proj;

		if (!Frustum(model_to_proj).Contains(model.GetAABB()))
			return;

		model.Bind(device_context);

		RenderModel(model, model_to_proj);
	});
}


void XM_CALLCONV DepthPass::UpdateCamera(FXMMATRIX world_to_camera, CXMMATRIX camera_to_projection) {

	AltCameraBuffer buffer;
	buffer.world_to_camera      = XMMatrixTranspose(world_to_camera);
	buffer.camera_to_projection = XMMatrixTranspose(camera_to_projection);

	alt_cam_buffer.UpdateData(device_context, buffer);
	alt_cam_buffer.Bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA_ALT);
}


void XM_CALLCONV DepthPass::RenderModel(Model& model, FXMMATRIX model_to_projection) {

	model.ForEachChild([&](ModelChild& child) {
		//if (child.GetMaterial().transparent
		//	|| child.GetMaterial().dissolve > THRESHOLD)
		//	return;

		if (!child.CastsShadows())
			return;

		if (!Frustum(model_to_projection).Contains(child.GetAABB()))
			return;

		child.BindBuffer<Pipeline::VS>(device_context, SLOT_CBUFFER_MODEL);

		model.Draw(device_context, child.GetIndexCount(), child.GetIndexStart());
	});
}
