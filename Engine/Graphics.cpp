#include "stdafx.h"
#include "Graphics.h"

// TODO:
// - Separate shader bind and draw functions to eliminate unnecessary calls to SetShader
// - Design a method of associating models with shaders, so the following flow of logic can be implemented:
//		for (each shader) :
//			bind shader;
//			for (each object using shader) :
//				render object;

Graphics::Graphics(HWND hWnd, int windowWidth, int windowHeight):
	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight)
{
}


Graphics::~Graphics() {
}


bool Graphics::Init() {
	bool result;

	// Initialize Direct3D
	m_D3DApp = make_unique<Direct3D>(m_hWnd, m_WindowWidth, m_WindowHeight, MSAA_STATE, VSYNC_STATE, FULLSCREEN_STATE);
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
	m_ShaderMgr = make_unique<ShaderMgr>(m_Device, m_DeviceContext, m_hWnd);
	result = m_ShaderMgr->CreateShader(ShaderTypes::LightShader, L"./shaders/light/light.vs", L"./shaders/light/light.ps");
	if (!result) return false;
	

	// Create texture manager
	m_TextureMgr = make_unique<TextureMgr>(m_Device, m_DeviceContext);
	m_TextureMgr->CreateTexture(L"Brick", vector<wstring>(1, L"./data/brick.jpg"));
	m_TextureMgr->CreateSimpleTexture(wstring(L"Aqua"), XMFLOAT4(Colors::Aqua));


	// Create text manager
	m_TextMgr = make_unique<TextMgr>(m_Device, m_DeviceContext);
	m_TextMgr->CreateTextObject(L"FPS", L"./data/courier-16.spritefont");
	m_TextMgr->GetTextObject(L"FPS")->SetText(L"FPS: ", XMFLOAT2(10, 10));


	// Create light
	m_Light = make_unique<Light>();

	m_Light->SetDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_Light->SetAmbientColor(XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f));
	m_Light->SetDiffuseColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetSpecularPower(32.0f);


	// Create model
	m_Model = make_unique<Model>();
	result = m_Model->Init(m_Device, "./data/cube.txt", m_TextureMgr->GetTexture(L"Brick"));
	//result = m_Model->Init(m_Device, "./data/cube.txt", m_TextureMgr->GetTexture(L"Aqua"));
	if (!result) return false;

	return true;
}


bool Graphics::Tick(float deltaTime, int FPS) {
	m_D3DApp->BeginScene(0.39f, 0.58f, 0.93f, 1.0f);

	rotation += (XM_PI * deltaTime) / 2500;
	if (rotation >= (2.0f * XM_PI)) rotation = 0;

	m_Camera->Render();
	XMMATRIX world      = m_D3DApp->GetWorldMatrix();
	world = XMMatrixRotationRollPitchYaw(rotation, rotation, 0.0f);
	XMMATRIX projection = m_D3DApp->GetProjectionMatrix();
	XMMATRIX view       = m_Camera->GetViewMatrix();

	m_Model->RenderBuffers(m_DeviceContext);

	m_Shader = m_ShaderMgr->GetShader(ShaderTypes::LightShader);
	get<shared_ptr<LightShader>>(m_Shader)->Render(m_DeviceContext, m_Model->GetIndexCount(), world, view, projection, m_Camera->GetPosition(),
												   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
												   m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	m_TextMgr->GetTextObject(L"FPS")->SetText(L"FPS: " + std::to_wstring(FPS));
	m_TextMgr->GetTextObject(L"FPS")->Render();

	m_D3DApp->EndScene();

	return true;
}


void Graphics::OnResize(int windowWidth, int windowHeight) {
	if (!this) return;
	m_D3DApp->OnResize(windowWidth, windowHeight);
}

