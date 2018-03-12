#include "stdafx.h"
#include "renderer.h"


Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device_context(device_context)
	, camera_buffer(device)
{
	// Set cbuffers
	device_context->PSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer.GetBufferAddress());
	device_context->VSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer.GetBufferAddress());

	// Create forward renderer
	forward_renderer = make_unique<ForwardRenderer>(device, device_context);
}


void Renderer::Render(Scene& scene, RenderStateMgr& render_state_mgr) const {

	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	camera_buffer.UpdateData(device_context.Get(), CameraBuffer(scene.camera->GetPosition()));

	
	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
	forward_renderer->Render(scene, render_state_mgr);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});
}
