#include "stdafx.h"
#include "Scene.h"

// TODO:
// - DONE - Create a class for sampler states
// - PARTIAL - Method for setting sampler state
// - DONE - Create a material struct for models (diffuse, ambient, specular, etc values)
// - Implement material data into shaders
// - Create frustum and implement frustum culling
// - DONE - Create and implement input handler
// - Create a model loader, and a model class that can take any vertex type
// - Create a simple geometry generator

Scene::Scene(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_hWnd(hWnd),
	m_Device(device),
	m_DeviceContext(deviceContext),
	m_Rotation(0.0f, 0.0f, 0.0f)
{
}


Scene::~Scene() {
}


bool Scene::Init() {
	bool result;

	//----------------------------------------------------------------------------------
	// Create camera
	//----------------------------------------------------------------------------------
	m_Camera = make_unique<Camera>();
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));


	//----------------------------------------------------------------------------------
	// Create texture manager
	//----------------------------------------------------------------------------------
	m_TextureMgr = make_unique<TextureMgr>(m_Device, m_DeviceContext);


	//----------------------------------------------------------------------------------
	// Create lights
	//----------------------------------------------------------------------------------
	m_Lights.push_back(Light());
	m_Lights[0].SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_Lights[0].SetAmbientColor(XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f));
	m_Lights[0].SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Lights[0].SetSpecularPower(32.0f);


	//----------------------------------------------------------------------------------
	// Create models
	//----------------------------------------------------------------------------------
	m_Models.push_back(Model());
	vector<wstring> v;
	v.push_back(L"./data/brick.jpg");
	//v.push_back(L"./data/brick2.jpg");
	result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->Texture(v), ShaderTypes::LightShader);
	//result = m_Models[0].Init(m_Device, "./data/cube.txt", m_TextureMgr->SimpleTexture(XMFLOAT4(Colors::Aqua)), ShaderTypes::LightShader);
	if (!result) return false;


	//----------------------------------------------------------------------------------
	// Create text objects
	//----------------------------------------------------------------------------------
	m_Texts.try_emplace(L"FPS", m_Device, m_DeviceContext, L"./data/courier-12.spritefont");
	m_Texts.at(L"FPS").SetPosition(XMFLOAT2(10, 10));

	m_Texts.try_emplace(L"Mouse X", m_Device, m_DeviceContext, L"./data/courier-12.spritefont");
	m_Texts.at(L"Mouse X").SetPosition(XMFLOAT2(10, 30));

	m_Texts.try_emplace(L"Mouse Y", m_Device, m_DeviceContext, L"./data/courier-12.spritefont");
	m_Texts.at(L"Mouse Y").SetPosition(XMFLOAT2(10, 50));

	return true;
}


void Scene::UpdateMetrics(int FPS, int CPU, int mouseX, int mouseY) {
	// FPS, CPU usage, memory usage, mouse position, etc...

	m_Texts.at(L"FPS").SetText(L"FPS: " + to_wstring(FPS));
	m_Texts.at(L"Mouse X").SetText(L"Mouse X: " + to_wstring(mouseX));
	m_Texts.at(L"Mouse Y").SetText(L"Mouse Y: " + to_wstring(mouseY));
}


void Scene::Tick(Input& input, float deltaTime) {
	XMFLOAT3 rotationRates(0.0f, 0.0f, 0.0f);

	if (input.IsKeyPressed(DIK_W)) {
		rotationRates.x -= deltaTime * 0.015f;
	}
	else if (input.IsKeyPressed(DIK_S)) {
		rotationRates.x += deltaTime * 0.015f;
	}

	if (input.IsKeyPressed(DIK_A)) {
		rotationRates.y -= deltaTime * 0.015f;
	}
	else if (input.IsKeyPressed(DIK_D)) {
		rotationRates.y += deltaTime * 0.015f;
	}

	// Set camera rotation
	m_Camera->Rotate(rotationRates);

	// Update camera 
	m_Camera->Render();
}