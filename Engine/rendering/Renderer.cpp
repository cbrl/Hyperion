#include "stdafx.h"
#include "renderer.h"
#include "system\system.h"
#include "rendering\rendering_mgr.h"


const Renderer* Renderer::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetRenderer();
}


Renderer::Renderer() : camera_buffer(RenderingMgr::GetDevice()) {
	// Set cbuffers
	RenderingMgr::GetDeviceContext()->PSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer.GetBufferAddress());
	RenderingMgr::GetDeviceContext()->VSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer.GetBufferAddress());

	// Create forward renderer
	forward_renderer = make_unique<ForwardRenderer>();
}


Renderer::~Renderer() {
}


void Renderer::Tick(Scene& scene, float deltaTime) const {

	//----------------------------------------------------------------------------------
	// Clear background with specified color
	//----------------------------------------------------------------------------------

	Direct3D::Get()->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);


	//----------------------------------------------------------------------------------
	// Update the camera buffer
	//----------------------------------------------------------------------------------

	camera_buffer.UpdateData(RenderingMgr::GetDeviceContext(), CameraBuffer(scene.camera->GetPosition()));

	
	//----------------------------------------------------------------------------------
	// Render objects with forward shader
	//----------------------------------------------------------------------------------
	
	forward_renderer->Render(scene);


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------

	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});


	//----------------------------------------------------------------------------------
	// Present the frame
	//----------------------------------------------------------------------------------

	Direct3D::Get()->EndScene();
}
