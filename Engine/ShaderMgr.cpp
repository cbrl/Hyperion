#include "stdafx.h"
#include "ShaderMgr.h"


ShaderMgr::ShaderMgr(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, HWND hWnd) :
	m_Device(device),
	m_DeviceContext(deviceContext),
	m_hWnd(hWnd)
{
}


ShaderMgr::~ShaderMgr() {
}


bool ShaderMgr::Init() {
	bool result;

	m_LightShader = make_unique<LightShader>();
	result = m_LightShader->Init(m_Device, m_hWnd, L"./shaders/light/light.vs", L"./shaders/light/light.ps");

	// Other shader init stuff here

	return result;
}
