#include "stdafx.h"
#include "renderer.h"
#include "system\system.h"
#include "rendering\rendering_mgr.h"


const Renderer* Renderer::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetRenderer();
}


Renderer::Renderer()
	: device(RenderingMgr::GetDevice())
	, device_context(RenderingMgr::GetDeviceContext())
	, camera_buffer(device.Get())
{
	forward_renderer = make_unique<ForwardRenderer>();
}


Renderer::~Renderer() {
}


void Renderer::Tick(Scene& scene, float deltaTime) const {
	// Clear background with specified color
	Direct3D::Get()->BeginScene(0.6f, 0.6f, 0.6f, 1.0f);

	//// Create matrix buffer
	//MatrixBuffer matrix_buffer;

	//// Get matrices
	//XMMATRIX world;
	//XMMATRIX view = scene.camera->GetViewMatrix();
	//XMMATRIX projection = Direct3D::Get()->GetProjectionMatrix();

	//// Create the frustum
	//Frustum frustum(view*projection);

	//// Transpose matrices for shader
	//view = XMMatrixTranspose(view);
	//projection = XMMatrixTranspose(projection);

	
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


	// Present the frame
	Direct3D::Get()->EndScene();
}
