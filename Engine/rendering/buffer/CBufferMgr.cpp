#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr(ID3D11Device* device, ComPtr<ID3D11DeviceContext> deviceContext) :
	deviceContext(deviceContext)
{
	CreateBuffers(device);
}


CBufferMgr::CBufferMgr() {
}


void CBufferMgr::CreateBuffers(ID3D11Device* device) {
	matrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

	cameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

	lightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
}


void CBufferMgr::BindCBuffer(BufferTypes buffer) {
	switch (buffer) {
		case BufferTypes::MatrixBuffer:
			break;

		case BufferTypes::CameraBuffer:
			break;

		case BufferTypes::LightBuffer:
			deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->buffer.GetAddressOf());
			deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->buffer.GetAddressOf());
			deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->buffer.GetAddressOf());
	}
}