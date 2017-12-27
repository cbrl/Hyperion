#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_hWnd(hWnd),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


Renderer::~Renderer() {
}


ComPtr<ID3D11Device> Renderer::GetDevice() {
	return m_Device;
}


ComPtr<ID3D11DeviceContext> Renderer::GetDeviceContext() {
	return m_DeviceContext;
}


bool Renderer::Tick(Direct3D& direct3D, Scene& scene, float deltaTime) {
	direct3D.BeginScene(0.39f, 0.58f, 0.93f, 1.0f);

	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;

	scene.m_Camera->Render();
	XMMATRIX world      = direct3D.GetWorldMatrix();
	world = XMMatrixRotationRollPitchYaw(rotation, rotation, 0.0f);
	XMMATRIX projection = direct3D.GetProjectionMatrix();
	XMMATRIX view       = scene.m_Camera->GetViewMatrix();

	m_Shader = scene.m_ShaderMgr->GetShader(ShaderTypes::LightShader);
	Light *light = &scene.m_Lights.front();

	/*scene.ForEach<Model>([this, world, projection, view, light](Model& model) {
	model.RenderBuffers(m_DeviceContext);

	get<shared_ptr<LightShader>>(m_Shader)->Render(m_DeviceContext, model.GetIndexCount(), world, view, projection, m_Camera->GetPosition(),
	model.GetTexture(), light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(),
	light->GetSpecularColor(), light->GetSpecularPower());
	});*/

	get<shared_ptr<LightShader>>(m_Shader)->SetShader(m_DeviceContext);
	get<shared_ptr<LightShader>>(m_Shader)->SetCBuffers(m_DeviceContext);
	
	for (Model& model : scene.m_Models) {
		model.RenderBuffers(m_DeviceContext);

		get<shared_ptr<LightShader>>(m_Shader)->SetParameters(m_DeviceContext, world, view, projection, scene.m_Camera->GetPosition(),
															  model.GetTexture(), *light);
		get<shared_ptr<LightShader>>(m_Shader)->Render(m_DeviceContext, model.GetIndexCount());
	}


	for (auto& kv : scene.m_Texts) {
		kv.second.Render();
	}

	direct3D.EndScene();

	return true;
}
