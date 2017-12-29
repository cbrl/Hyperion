#include "stdafx.h"
#include "Scene.h"

// TODO:
// - Create a class for sampler states
// - Create a material struct for models (diffuse, ambient, specular, etc values)
// - Create a simple geometry generator

Scene::Scene(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_hWnd(hWnd),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


Scene::~Scene() {
}


bool Scene::Init() {
	bool result;

	// Create camera
	m_Camera = make_unique<Camera>();
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));


	// Create texture manager
	m_TextureMgr = make_unique<TextureMgr>(m_Device, m_DeviceContext);


	// Create lights
	m_Lights.push_back(Light());
	m_Lights[0].SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_Lights[0].SetAmbientColor(XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f));
	m_Lights[0].SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularPower(32.0f);


	// Create models
	m_Models.push_back(Model());
	result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->Texture(vector<wstring>(1, L"./data/brick.jpg")), ShaderTypes::LightShader);
	//result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->SimpleTexture(XMFLOAT4(Colors::Aqua)), ShaderTypes::LightShader);
	if (!result) return false;


	// Create text objects
	m_Texts.try_emplace(L"FPS", m_Device, m_DeviceContext, L"./data/courier-12.spritefont");
	m_Texts.at(L"FPS").SetPosition(XMFLOAT2(10, 10));

	return true;
}


void Scene::UpdateMetrics(int FPS, int CPU) {
	// FPS, CPU usage, memory usage, mouse position, etc...

	m_Texts.at(L"FPS").SetText(L"FPS: " + std::to_wstring(FPS));
}