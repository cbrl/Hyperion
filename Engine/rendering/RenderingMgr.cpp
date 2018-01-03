#include "stdafx.h"
#include "RenderingMgr.h"


RenderingMgr::RenderingMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_hWnd(hWnd),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


RenderingMgr::~RenderingMgr() {
}


bool RenderingMgr::Init() {
	bool result;

	// Create buffer manager
	m_CBufferMgr = make_unique<CBufferMgr>(m_Device, m_DeviceContext);


	// Create shaders
	m_LightShader = make_unique<Shader>();
	result = m_LightShader->Init(m_hWnd, m_Device, L"./shaders/light/light.vs", L"./shaders/light/light.ps",
								 VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);
	if (!result) return false;

	// Other shader init stuff here

	return true;
}

void RenderingMgr::BindShader(ShaderTypes shader) {
	switch (shader) {
		case ShaderTypes::LightShader:
			m_LightShader->BindShader(m_DeviceContext);
			m_CBufferMgr->SetCBuffer(BufferTypes::MatrixBuffer);
			m_CBufferMgr->SetCBuffer(BufferTypes::CameraBuffer);
			m_CBufferMgr->SetCBuffer(BufferTypes::LightBuffer);
			break;

		// Other cases here
	}
}
