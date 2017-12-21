#include "stdafx.h"
#include "System.h"


System::System(HWND hWnd, int windowWidth, int windowHeight):
	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight),
	m_D3DApp(nullptr)
{
}


System::~System() {
}


bool System::Init() {
	// Initialize Direct3D
	m_D3DApp = make_unique<Direct3D>(m_hWnd, m_WindowWidth, m_WindowHeight, false, true, false);
	if (!m_D3DApp->Init()) {
		return false;
	}

	// Get device and device context
	m_Device = m_D3DApp->GetDevice();
	m_DeviceContext = m_D3DApp->GetDeviceContext();


	// Create camera
	m_Camera = make_unique<Camera>();
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));

	// Create shader manager
	m_ShaderMgr = make_unique<ShaderMgr>(m_D3DApp->GetDevice(), m_D3DApp->GetDeviceContext(), m_hWnd);
	m_ShaderMgr->CreateShader(ShaderTypes::LightShader, L"LightShader", L"./shaders/light/light.vs", "LightVertexShader", L"./shaders/light/light.ps", "LightPixelShader");
	
	// Create texture manager
	m_TextureMgr = make_unique<TextureMgr>(m_D3DApp->GetDevice(), m_D3DApp->GetDeviceContext());

	// Create model
	m_Model = make_unique<Model>();
	m_Model->Init(m_Device, "./data/cube.txt", m_TextureMgr->CreateTexture(vector<wstring>(1, L"./data/brick.jpg")));

	return true;
}


bool System::Tick() {
	m_D3DApp->BeginScene(0.39f, 0.58f, 0.93f, 1.0f);

	//rotation += XM_PI / 250;
	//if (rotation > (2.0f * XM_PI)) rotation = 0;

	//m_Camera->Render();

	//XMMATRIX world      = m_D3DApp->GetWorldMatrix();
	//world = XMMatrixRotationRollPitchYaw(rotation, rotation, 0.0f);
	//XMMATRIX projection = m_D3DApp->GetProjectionMatrix();
	//XMMATRIX view       = m_Camera->GetViewMatrix();

	//m_Model->RenderBuffers(m_DeviceContext);

	// Light shader function here

	m_D3DApp->EndScene();

	return true;
}


void System::OnResize(int windowWidth, int windowHeight) {
	if (!this) return;
	if (m_D3DApp) {
		m_D3DApp->OnResize(windowWidth, windowHeight);
	}
}

