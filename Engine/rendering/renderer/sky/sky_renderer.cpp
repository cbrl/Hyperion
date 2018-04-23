#include "stdafx.h"
#include "sky_renderer.h"


SkyRenderer::SkyRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device(device)
	, device_context(device_context)
{

	vertex_shader = make_unique<VertexShader>(device, L"shaders/skybox/skybox_vs.hlsl",
											  VertexPosition::InputElements,
											  VertexPosition::InputElementCount);

	pixel_shader = make_unique<PixelShader>(device, L"shaders/skybox/skybox.hlsl");
}


void SkyRenderer::Render(Scene& scene, const RenderStateMgr& render_state_mgr) {

	// Get a reference to the camera's skybox
	const SkyBox& skybox = scene.GetCamera().GetSkybox();


	// Bind the render states
	BindRenderStates(scene, render_state_mgr);


	// Bind the shaders
	vertex_shader->Bind(device_context.Get());
	pixel_shader->Bind(device_context.Get());


	// Bind the texture
	auto texture = skybox.GetTexture();
	if (texture) texture->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SKYBOX);


	// Render the skybox
	skybox.Bind(device_context.Get());
	skybox.Draw(device_context.Get());
}


void SkyRenderer::BindRenderStates(Scene & scene, const RenderStateMgr & render_state_mgr) {

	switch (scene.GetRenderStates().blend_state) {
		case BlendStates::Default:
		case BlendStates::Opaque:
			render_state_mgr.BindOpaque(device_context.Get());
			break;
		case BlendStates::AlphaBlend:
			render_state_mgr.BindAlphaBlend(device_context.Get());
			break;
		case BlendStates::Additive:
			render_state_mgr.BindAdditive(device_context.Get());
			break;
		case BlendStates::NonPremultiplied:
			render_state_mgr.BindNonPremultiplied(device_context.Get());
			break;
	}

	switch (scene.GetRenderStates().depth_state) {
		case DepthStates::Default:
		case DepthStates::DepthDefault:
			render_state_mgr.BindDepthDefault(device_context.Get());
			break;
		case DepthStates::DepthNone:
			render_state_mgr.BindDepthNone(device_context.Get());
			break;
		case DepthStates::DepthRead:
			render_state_mgr.BindDepthRead(device_context.Get());
			break;
	}

	switch (scene.GetRenderStates().raster_state) {
		case RasterStates::Default:
		case RasterStates::CullNone:
			render_state_mgr.BindCullNone(device_context.Get());
			break;
		case RasterStates::CullClockwise:
			render_state_mgr.BindCullClockwise(device_context.Get());
			break;
		case RasterStates::CullCounterClockwise:
			render_state_mgr.BindCullCounterClockwise(device_context.Get());
			break;
		case RasterStates::Wireframe:
			render_state_mgr.BindWireframe(device_context.Get());
	}
}
