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


	//----------------------------------------------------------------------------------
	// Bind the render states
	//----------------------------------------------------------------------------------

	render_state_mgr.BindDepthDefault(device_context.Get());
	render_state_mgr.BindCullNone(device_context.Get());


	//----------------------------------------------------------------------------------
	// Bind the shaders
	//----------------------------------------------------------------------------------

	vertex_shader->Bind(device_context.Get());
	pixel_shader->Bind(device_context.Get());


	//----------------------------------------------------------------------------------
	// Bind the texture
	//----------------------------------------------------------------------------------

	auto texture = skybox.GetTexture();
	if (texture) texture->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SKYBOX);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	skybox.Bind(device_context.Get());
	skybox.Draw(device_context.Get());
}