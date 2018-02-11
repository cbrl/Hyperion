#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr() {
	CreateBuffers(Direct3D::Get()->GetDevice());
	BindBuffers(Direct3D::Get()->GetDeviceContext());
}


CBufferMgr::~CBufferMgr() {
}


void CBufferMgr::CreateBuffers(ID3D11Device* device) {
	matrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

	cameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

	lightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
}


void CBufferMgr::BindBuffers(ID3D11DeviceContext* deviceContext) {
	deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->GetBufferAddress());
	deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->GetBufferAddress());
	deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->GetBufferAddress());
}


void CBufferMgr::BindBuffer(ID3D11DeviceContext* deviceContext, BufferTypes buffer) {
	switch (buffer) {
		case BufferTypes::MatrixBuffer:
			deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->GetBufferAddress());
			break;

		case BufferTypes::CameraBuffer:
			deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->GetBufferAddress());
			break;

		case BufferTypes::LightBuffer:
			deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->GetBufferAddress());
			break;
	}
}