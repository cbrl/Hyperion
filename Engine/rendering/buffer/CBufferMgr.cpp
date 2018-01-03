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


void CBufferMgr::SetCBuffer(BufferTypes buffer) {
	switch (buffer) {
		case BufferTypes::MatrixBuffer:
			break;

		case BufferTypes::CameraBuffer:
			break;

		case BufferTypes::LightBuffer:
			m_DeviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, m_MatrixBuffer->buffer.GetAddressOf());
			m_DeviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, m_CameraBuffer->buffer.GetAddressOf());
			m_DeviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, m_LightBuffer->buffer.GetAddressOf());
	}
}