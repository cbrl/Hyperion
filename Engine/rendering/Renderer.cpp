#include "stdafx.h"
#include "Renderer.h"
#include "rendering\RenderingMgr.h"


const Renderer* Renderer::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetRenderer();
}


Renderer::Renderer() :
	device(Direct3D::Get()->GetDevice()),
	deviceContext(Direct3D::Get()->GetDeviceContext())
{
}


Renderer::~Renderer() {
}


void Renderer::Tick(Scene& scene, float deltaTime) const {
	// Clear background with specified color
	Direct3D::Get()->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);
	RenderStateMgr::Get()->BindDepthDefault(deviceContext.Get());

	// Create matrix buffer
	MatrixBuffer matrixBuffer;

	// Get matrices
	XMMATRIX world;
	XMMATRIX view = scene.camera->GetViewMatrix();
	XMMATRIX projection = Direct3D::Get()->GetProjectionMatrix();

	// Create the frustum
	Frustum frustum(view*projection);

	// Transpose matrices for shader
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);


	//----------------------------------------------------------------------------------
	// Render objects with color shader
	//----------------------------------------------------------------------------------

	// Bind shader
	RenderingMgr::Get()->BindShader(ShaderTypes::ColorShader);

	// Update cbuffers
	RenderingMgr::Get()->UpdateData(scene.camera->GetBuffer());

	// Render models
	scene.ForEach<Model>([&](Model& model){
		if (frustum.Contains(model.GetAABB())) {
			if (model.GetShader() == ShaderTypes::ColorShader) {
				// Get world matrix
				world = Direct3D::Get()->GetWorldMatrix();

				// Transform world matrix according to model position and rotation
				world = XMMatrixMultiply(world, model.GetRotation());
				world = XMMatrixMultiply(world, model.GetPosition());
				world = XMMatrixTranspose(world);

				// Create matrix buffer and update cbuffer
				matrixBuffer = MatrixBuffer(world, view, projection);
				RenderingMgr::Get()->UpdateData(matrixBuffer);

				model.Draw(deviceContext.Get());
			}
		}
	});

	
	//----------------------------------------------------------------------------------
	// Render objects with light shader
	//----------------------------------------------------------------------------------
	RenderingMgr::Get()->BindShader(ShaderTypes::LightShader);

	RenderingMgr::Get()->UpdateData(scene.camera->GetBuffer());
	RenderingMgr::Get()->UpdateData(scene.lights.front().GetBuffer());

	scene.ForEach<Model>([&](Model& model) {
		if (frustum.Contains(model.GetAABB()))
		{
			if (model.GetShader() == ShaderTypes::LightShader) {

				world = Direct3D::Get()->GetWorldMatrix();

				world = XMMatrixMultiply(world, model.GetRotation());
				world = XMMatrixMultiply(world, model.GetPosition());
				world = XMMatrixTranspose(world);

				matrixBuffer = MatrixBuffer(world, view, projection);
				RenderingMgr::Get()->UpdateData(matrixBuffer);

				model.Draw(deviceContext.Get());
			}
		}
	});


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------
	scene.ForEach<Text>([](Text& text) {
		text.Render();
	});


	// Present the frame
	Direct3D::Get()->EndScene();
}
