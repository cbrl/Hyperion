#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(HWND hWnd, shared_ptr<Direct3D> direct3D) :
	m_hWnd(hWnd),
	m_Direct3D(direct3D),
	m_Device(direct3D->GetDevice()),
	m_DeviceContext(direct3D->GetDeviceContext())
{
}


Renderer::~Renderer() {
}


bool Renderer::Init() {
	// Create rendering manager
	m_RenderingMgr = make_unique<RenderingMgr>(m_hWnd, m_Device, m_DeviceContext);
	if (!m_RenderingMgr->Init()) {
		return false;
	}

	return true;
}


bool Renderer::Tick(Scene& scene, float deltaTime) {
	// Clear background with specified color
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.93f, 1.0f);


	// Get matrices
	XMMATRIX world      = m_Direct3D->GetWorldMatrix();
	XMMATRIX projection = m_Direct3D->GetProjectionMatrix();
	XMMATRIX view       = scene.m_Camera->GetViewMatrix();


	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;
	world = XMMatrixRotationRollPitchYaw(rotation, rotation, 0.0f);
	

	// Transpose matrices for shader
	world      = XMMatrixTranspose(world);
	view       = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	// Create matrix m_Buffer
	MatrixBuffer matrixBuffer = MatrixBuffer(world, view, projection);


	//----------------------------------------------------------------------------------
	// Render objects with color shader
	//----------------------------------------------------------------------------------

	// Bind shader
	m_RenderingMgr->BindShader(ShaderTypes::ColorShader);

	// Update cbuffers
	m_RenderingMgr->UpdateData(matrixBuffer);
	m_RenderingMgr->UpdateData(scene.m_Camera->GetBuffer());

	// Render models
	for (Model& model : scene.m_Models) {
		if (model.GetShader() == ShaderTypes::ColorShader) {

			model.RenderBuffers(m_DeviceContext);
			m_DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
		}
	}


	//----------------------------------------------------------------------------------
	// Render objects with light shader
	//----------------------------------------------------------------------------------
	
	// Bind shader
	m_RenderingMgr->BindShader(ShaderTypes::LightShader);

	// Update cbuffers
	m_RenderingMgr->UpdateData(matrixBuffer);
	m_RenderingMgr->UpdateData(scene.m_Camera->GetBuffer());
	m_RenderingMgr->UpdateData(scene.m_Lights.front().GetBuffer());

	// Render models
	for (Model& model : scene.m_Models) {
		if (model.GetShader() == ShaderTypes::LightShader) {

			model.RenderBuffers(m_DeviceContext);
			m_DeviceContext->PSSetShaderResources(0, 1, model.GetTexture().GetAddressOf());
			m_DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
		}
	}


	//----------------------------------------------------------------------------------
	// Render text objects
	//----------------------------------------------------------------------------------
	for (auto& kv : scene.m_Texts) {
		kv.second.Render();
	}


	// Present the frame
	m_Direct3D->EndScene();

	return true;
}
