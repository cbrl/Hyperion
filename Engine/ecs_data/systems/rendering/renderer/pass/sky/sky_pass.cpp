#include "stdafx.h"
#include "sky_pass.h"
#include "engine\engine.h"


SkyPass::SkyPass(ID3D11Device& device, ID3D11DeviceContext& device_context)
	: device(device)
	, device_context(device_context)
{

	vertex_shader = make_unique<VertexShader>(device,
											  L"shaders/skybox/skybox_vs.hlsl",
											  VertexPosition::InputElements,
											  VertexPosition::InputElementCount);

	pixel_shader = make_unique<PixelShader>(device, L"shaders/skybox/skybox.hlsl");
}


void SkyPass::Render(const Engine& engine) {

	auto& ecs_engine             = engine.GetECS();
	const auto& render_state_mgr = engine.GetRenderingMgr().GetRenderStateMgr();
	auto& scene                  = engine.GetScene();

	// Get the camera's skybox
	const SkyBox* skybox = ecs_engine.GetComponent<SkyBox>(scene.GetCamera());
	if (!skybox) return;


	// Bind the render states
	BindRenderStates(scene, render_state_mgr);


	// Bind the shaders
	vertex_shader->Bind(device_context);
	pixel_shader->Bind(device_context);


	// Bind the texture
	auto texture = skybox->GetTexture();
	if (texture) texture->Bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);


	// Render the skybox
	skybox->Bind(device_context);
	skybox->Draw(device_context);
}


void SkyPass::BindRenderStates(Scene& scene, const RenderStateMgr& render_state_mgr) {

	switch (scene.GetRenderStates().blend_state) {
		case BlendStates::Default:
		case BlendStates::Opaque:
			render_state_mgr.BindOpaque(device_context);
			break;
		case BlendStates::AlphaBlend:
			render_state_mgr.BindAlphaBlend(device_context);
			break;
		case BlendStates::Additive:
			render_state_mgr.BindAdditive(device_context);
			break;
		case BlendStates::NonPremultiplied:
			render_state_mgr.BindNonPremultiplied(device_context);
			break;
	}

	switch (scene.GetRenderStates().depth_state) {
		case DepthStates::Default:
		case DepthStates::DepthDefault:
			render_state_mgr.BindDepthDefault(device_context);
			break;
		case DepthStates::DepthNone:
			render_state_mgr.BindDepthNone(device_context);
			break;
		case DepthStates::DepthRead:
			render_state_mgr.BindDepthRead(device_context);
			break;
	}

	switch (scene.GetRenderStates().raster_state) {
		case RasterStates::Default:
		case RasterStates::CullNone:
			render_state_mgr.BindCullNone(device_context);
			break;
		case RasterStates::CullClockwise:
			render_state_mgr.BindCullClockwise(device_context);
			break;
		case RasterStates::CullCounterClockwise:
			render_state_mgr.BindCullCounterClockwise(device_context);
			break;
		case RasterStates::Wireframe:
			render_state_mgr.BindWireframe(device_context);
	}
}