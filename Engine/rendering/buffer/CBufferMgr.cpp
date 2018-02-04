#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr(ID3D11Device* device, ComPtr<ID3D11DeviceContext> deviceContext) :
	deviceContext(deviceContext)
{
	CreateBuffers(device);
	BindBuffers();
}


CBufferMgr::CBufferMgr() {
}


void CBufferMgr::CreateBuffers(ID3D11Device* device) {
	matrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

	cameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

	lightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
}


void CBufferMgr::BindBuffers() {
	deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->buffer.GetAddressOf());
	deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->buffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->buffer.GetAddressOf());
}


void CBufferMgr::BindBuffer(BufferTypes buffer) {
	switch (buffer) {
		case BufferTypes::MatrixBuffer:
			deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->buffer.GetAddressOf());
			break;

		case BufferTypes::CameraBuffer:
			deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->buffer.GetAddressOf());
			break;

		case BufferTypes::LightBuffer:
			deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->buffer.GetAddressOf());
			break;
	}
}