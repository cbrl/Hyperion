#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr(const ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_DeviceContext(deviceContext)
{
	CreateBuffers(device);
}


CBufferMgr::CBufferMgr() {
}


void CBufferMgr::CreateBuffers(const ComPtr<ID3D11Device>& device) {
	m_MatrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

	m_CameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

	m_LightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
}


void CBufferMgr::BindCBuffer(BufferTypes m_Buffer) {
	switch (m_Buffer) {
		case BufferTypes::MatrixBuffer:
			break;

		case BufferTypes::CameraBuffer:
			break;

		case BufferTypes::LightBuffer:
			m_DeviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, m_MatrixBuffer->m_Buffer.GetAddressOf());
			m_DeviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, m_CameraBuffer->m_Buffer.GetAddressOf());
			m_DeviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, m_LightBuffer->m_Buffer.GetAddressOf());
	}
}