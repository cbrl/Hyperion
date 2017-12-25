#include "stdafx.h"
#include "Scene.h"

// TODO:
// - Separate shader bind and draw functions to eliminate unnecessary calls to SetShader
// - Design a method of associating models with shaders, so the following flow of logic can be implemented:
//		for (each shader) :
//			bind shader;
//			for (each object using shader) :
//				render object;
// - Possibly create bind function in shader manager

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


	// Create shader manager
	m_ShaderMgr = make_unique<ShaderMgr>(m_Device, m_DeviceContext, m_hWnd);
	result = m_ShaderMgr->CreateShader(ShaderTypes::LightShader, L"./shaders/light/light.vs", L"./shaders/light/light.ps");
	if (!result) return false;


	// Create texture manager
	m_TextureMgr = make_unique<TextureMgr>(m_Device, m_DeviceContext);


	// Create lights
	m_Lights.push_back(Light());
	m_Lights[0].SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_Lights[0].SetAmbientColor(XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f));
	m_Lights[0].SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularPower(16.0f);


	// Create models
	m_Models.push_back(Model());
	//result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->Texture(vector<wstring>(1, L"./data/brick.jpg")));
	result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->SimpleTexture(L"Aqua", XMFLOAT4(Colors::Aqua)));
	if (!result) return false;


	// Create text objects
	m_Texts.try_emplace(L"FPS", m_Device, m_DeviceContext, L"./data/courier-12.spritefont");

	return true;
}


void Scene::UpdateMetrics(int FPS, int CPU) {
	// CPU usage, memory usage, mouse position, etc...

	m_Texts.at(L"FPS").SetText(L"FPS: " + std::to_wstring(FPS), XMFLOAT2(10, 10));
}