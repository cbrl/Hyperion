#include "stdafx.h"
#include "ShaderMgr.h"


ShaderMgr::ShaderMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_hWnd(hWnd),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


ShaderMgr::~ShaderMgr() {
}


bool ShaderMgr::Init() {
	bool result;

	m_CBuffers = make_unique<CBuffers>(m_Device);

	m_LightShader = make_unique<Shader>();
	result = m_LightShader->Init(m_hWnd, m_Device, L"./shaders/light/light.vs", L"./shaders/light/light.ps",
								  VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);

	// Other shader init stuff here

	return true;
}

void ShaderMgr::BindShader(ShaderTypes shader) {
	switch (shader) {
		case ShaderTypes::LightShader:
			m_LightShader->BindShader(m_DeviceContext);
			m_DeviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, m_CBuffers->matrixBuffer->m_Buffer.GetAddressOf());
			m_DeviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, m_CBuffers->cameraBuffer->m_Buffer.GetAddressOf());
			m_DeviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, m_CBuffers->lightBuffer->m_Buffer.GetAddressOf());
			break;

		// Other cases here
	}
}
