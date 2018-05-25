#include "stdafx.h"
#include "sky_pass.h"
#include "rendering\render_state_mgr.h"
#include "scene\scene.h"


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


void SkyPass::Render(const SkyBox& skybox) const {

	// Bind the shaders
	vertex_shader->Bind(device_context);
	pixel_shader->Bind(device_context);


	// Bind the texture
	auto texture = skybox.GetTexture();
	if (texture) texture->Bind<Pipeline::PS>(device_context, SLOT_SRV_SKYBOX);


	// Render the skybox
	skybox.Bind(device_context);
	skybox.Draw(device_context);
}


void SkyPass::BindDefaultRenderStates(const RenderStateMgr& render_state_mgr) const {

	Pipeline::DS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::GS::BindShader(device_context, nullptr, nullptr, 0);
	Pipeline::HS::BindShader(device_context, nullptr, nullptr, 0);

	render_state_mgr.BindOpaque(device_context);
	render_state_mgr.BindDepthDefault(device_context);
	render_state_mgr.BindCullNone(device_context);
}
