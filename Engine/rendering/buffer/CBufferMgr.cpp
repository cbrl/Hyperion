#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr() :
	deviceContext(Direct3D::Get()->GetDeviceContext())
{
	CreateBuffers(Direct3D::Get()->GetDevice());
	BindBuffers();
}


CBufferMgr::~CBufferMgr() {
}


void CBufferMgr::CreateBuffers(ID3D11Device* device) {
	matrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

	cameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

	lightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
}


void CBufferMgr::BindBuffers() {
	deviceContext->VSSetConstantBuffers(MATRIX_BUFFER_SLOT, 1, matrixBuffer->GetBufferAddress());
	deviceContext->VSSetConstantBuffers(CAMERA_BUFFER_SLOT, 1, cameraBuffer->GetBufferAddress());
	deviceContext->PSSetConstantBuffers(LIGHT_BUFFER_SLOT, 1, lightBuffer->GetBufferAddress());
}


void CBufferMgr::BindBuffer(BufferTypes buffer) {
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