#include "stdafx.h"
#include "renderer.h"

#include "util\engine_util.h"
#include "shader\hlsl.h"
#include "rendering\pipeline.h"


Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device_context(device_context)
	, camera_buffer(device)
{
	// Set cbuffers
	camera_buffer.Bind<Pipeline::PS>(device_context, SLOT_CBUFFER_CAMERA);
	camera_buffer.Bind<Pipeline::VS>(device_context, SLOT_CBUFFER_CAMERA);

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
