#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(HWND hWnd, shared_ptr<Direct3D> direct3D) :
	hWnd(hWnd),
	direct3D(direct3D),
	device(direct3D->GetDevice()),
	deviceContext(direct3D->GetDeviceContext())
{
}


Renderer::~Renderer() {
}


bool Renderer::Init() {
	// Create rendering manager
	renderingMgr = make_unique<RenderingMgr>(hWnd, device, deviceContext);
	if (!renderingMgr->Init()) {
		return false;
	}

	return true;
}


bool Renderer::Tick(Scene& scene, float deltaTime) {
	// Clear background with specified color
	direct3D->BeginScene(0.39f, 0.58f, 0.93f, 1.0f);


	// Get matrices
	XMMATRIX world      = direct3D->GetWorldMatrix();
	XMMATRIX projection = direct3D->GetProjectionMatrix();
	XMMATRIX view       = scene.camera->GetViewMatrix();

	// Create matrix buffer
	MatrixBuffer matrixBuffer;

	// Transpose matrices for shader
	view       = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);
	

	//----------------------------------------------------------------------------------
	// Render objects with color shader
	//----------------------------------------------------------------------------------
	// Bind shader
	renderingMgr->BindShader(ShaderTypes::ColorShader);

	// Update cbuffers
	renderingMgr->UpdateData(matrixBuffer);
	renderingMgr->UpdateData(scene.camera->GetBuffer());

	// Render models
	scene.ForEach<Model>([&](Model& model){
		if (model.GetShader() == ShaderTypes::ColorShader) {

			// Transform world matrix according to model position and rotation
			world = XMMatrixMultiply(world, model.GetRotation());
			world = XMMatrixMultiply(world, model.GetPosition());
			world = XMMatrixTranspose(world);

			// Create matrix buffer and update cbuffer
			matrixBuffer = MatrixBuffer(world, view, projection);
			renderingMgr->UpdateData(matrixBuffer);

			model.RenderBuffers(deviceContext.Get());
			deviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
		}
	});

	
	//----------------------------------------------------------------------------------
	// Render objects with light shader
	//----------------------------------------------------------------------------------
	renderingMgr->BindShader(ShaderTypes::LightShader);

	renderingMgr->UpdateData(scene.camera->GetBuffer());
	renderingMgr->UpdateData(scene.lights.front().GetBuffer());

	scene.ForEach<Model>([&](Model& model) {
		if (model.GetShader() == ShaderTypes::LightShader) {

			world = XMMatrixMultiply(world, model.GetRotation());
			world = XMMatrixMultiply(world, model.GetPosition());
			world = XMMatrixTranspose(world);

			matrixBuffer = MatrixBuffer(world, view, projection);
			renderingMgr->UpdateData(matrixBuffer);

			model.RenderBuffers(deviceContext.Get());
			deviceContext->PSSetShaderResources(0, 1, model.GetTexture().GetAddressOf());
			deviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
		}
	});


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------
	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});


	// Present the frame
	direct3D->EndScene();

	return true;
}
