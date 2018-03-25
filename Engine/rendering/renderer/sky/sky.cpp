#include "stdafx.h"
#include "sky.h"


SkyRenderer::SkyRenderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device(device)
	, device_context(device_context)
	, buffer(device)
{

	vertex_shader = make_unique<VertexShader>(device, L"shaders/skybox/skybox_vs.hlsl",
											  VertexPosition::InputElements,
											  VertexPosition::InputElementCount);

	pixel_shader = make_unique<PixelShader>(device, L"shaders/skybox/skybox.hlsl");
}


void SkyRenderer::Render(Scene& scene, const RenderStateMgr& render_state_mgr) {

	//----------------------------------------------------------------------------------
	// Bind the render states
	//----------------------------------------------------------------------------------

	render_state_mgr.BindDepthNone(device_context.Get());
	render_state_mgr.BindCullNone(device_context.Get());


	//----------------------------------------------------------------------------------
	// Bind the shaders and buffers
	//----------------------------------------------------------------------------------

	vertex_shader->Bind(device_context.Get());
	pixel_shader->Bind(device_context.Get());

	buffer.Bind<Pipeline::VS>(device_context.Get(), SLOT_CBUFFER_WVP);


	//----------------------------------------------------------------------------------
	// Update the buffer data
	//----------------------------------------------------------------------------------

	float3 pos = scene.camera->GetPosition();
	XMMATRIX world = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX M = XMMatrixTranspose(world * scene.camera->GetViewMatrix() * scene.camera->GetProjMatrix());

	WVPBuffer data(M);

	buffer.UpdateData(device_context.Get(), data);


	//----------------------------------------------------------------------------------
	// Bind the texture
	//----------------------------------------------------------------------------------

	scene.skybox.GetTexture()->Bind<Pipeline::PS>(device_context.Get(), SLOT_SRV_SKYBOX);


	//----------------------------------------------------------------------------------
	// Render the skybox
	//----------------------------------------------------------------------------------
	scene.skybox.Bind(device_context.Get());
	scene.skybox.Draw(device_context.Get());
}