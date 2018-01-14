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
	// Create buffer manager
	m_CBufferMgr = make_unique<CBufferMgr>(m_Device, m_DeviceContext);


	// Create sampler state
	m_Sampler = make_unique<Sampler>(m_Device);


	// Create shader manager
	m_ShaderMgr = make_unique<ShaderMgr>();
	if (!m_ShaderMgr->Init(m_hWnd, m_Device)) {
		return false;
	}

	return true;
}

void RenderingMgr::BindShader(ShaderTypes shader) {
	switch (shader) {
		case ShaderTypes::ColorShader:
			m_ShaderMgr->BindShader(ShaderTypes::ColorShader, m_DeviceContext);
			m_CBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			break;

		case ShaderTypes::TextureShader:
			//m_ShaderMgr->BindShader(ShaderTypes::TextureShader, m_DeviceContext);
			m_CBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			m_CBufferMgr->BindCBuffer(BufferTypes::CameraBuffer);
			m_DeviceContext->PSSetSamplers(0, 1, m_Sampler->samplerState.GetAddressOf());
			break;

		case ShaderTypes::LightShader:
			m_ShaderMgr->BindShader(ShaderTypes::LightShader, m_DeviceContext);
			m_CBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			m_CBufferMgr->BindCBuffer(BufferTypes::CameraBuffer);
			m_CBufferMgr->BindCBuffer(BufferTypes::LightBuffer);
			m_DeviceContext->PSSetSamplers(0, 1, m_Sampler->samplerState.GetAddressOf());
			break;

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}
