#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(HWND hWnd, shared_ptr<Direct3D> direct3D) :
	m_hWnd(hWnd),
	m_Direct3D(direct3D)
{
	m_Device        = direct3D->GetDevice();
	m_DeviceContext = direct3D->GetDeviceContext();
}


Renderer::~Renderer() {
}


bool Renderer::Init() {
	// Create shader manager
	m_ShaderMgr = make_unique<ShaderMgr>(m_hWnd, m_Device, m_DeviceContext);
	if (!m_ShaderMgr->Init()) {
		return false;
	}

	return true;
}


bool Renderer::Tick(Scene& scene, float deltaTime) {
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.93f, 1.0f);


	// Get matrices
	scene.m_Camera->Render();
	XMMATRIX world      = m_Direct3D->GetWorldMatrix();
	XMMATRIX projection = m_Direct3D->GetProjectionMatrix();
	XMMATRIX view       = scene.m_Camera->GetViewMatrix();


	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;
	world = XMMatrixRotationRollPitchYaw(rotation, rotation, 0.0f);
	

	// Transpose matrices for shader
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);


	// Render light shader
	m_ShaderMgr->BindShader(ShaderTypes::LightShader);
	for (Model& model : scene.m_Models) {

		if (model.GetShader() == ShaderTypes::LightShader) {

			model.RenderBuffers(m_DeviceContext);

			m_ShaderMgr->UpdateData(MatrixBuffer(world, view, projection));
			m_ShaderMgr->UpdateData(scene.m_Camera->GetBuffer());
			m_ShaderMgr->UpdateData(scene.m_Lights.front().GetBuffer());

			m_DeviceContext->PSSetShaderResources(0, 1, model.GetTexture().GetAddressOf());
			m_DeviceContext->DrawIndexed(model.GetIndexCount(), 0, 0);
		}
	}


	// Render text objects
	for (auto& kv : scene.m_Texts) {
		kv.second.Render();
	}


	m_Direct3D->EndScene();

	return true;
}
