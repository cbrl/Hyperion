#include "stdafx.h"
#include "CBufferMgr.h"


CBufferMgr::CBufferMgr() {
	CreateBuffers(Direct3D::Get()->GetDevice());
	BindBuffers(Direct3D::Get()->GetDeviceContext());
}


CBufferMgr::~CBufferMgr() {
}


void CBufferMgr::CreateBuffers(ID3D11Device* device) {
	matrix_buffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);
	camera_buffer = make_unique<ConstantBuffer<CameraBuffer>>(device);
	light_buffer = make_unique<ConstantBuffer<DirectionalLight>>(device);
}


void CBufferMgr::BindBuffers(ID3D11DeviceContext* device_context) const {
	device_context->VSSetConstantBuffers(SLOT_CBUFFER_MATRIX, 1, matrix_buffer->GetBufferAddress());
	device_context->VSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer->GetBufferAddress());
	device_context->PSSetConstantBuffers(SLOT_CBUFFER_LIGHT, 1, light_buffer->GetBufferAddress());
}


void CBufferMgr::BindBuffer(ID3D11DeviceContext* device_context, BufferTypes buffer) const {
	switch (buffer) {
		case BufferTypes::MatrixBuffer:
			device_context->VSSetConstantBuffers(SLOT_CBUFFER_MATRIX, 1, matrix_buffer->GetBufferAddress());
			break;

		case BufferTypes::CameraBuffer:
			device_context->VSSetConstantBuffers(SLOT_CBUFFER_CAMERA, 1, camera_buffer->GetBufferAddress());
			break;

		case BufferTypes::DirectionalLight:
			device_context->PSSetConstantBuffers(SLOT_CBUFFER_LIGHT, 1, light_buffer->GetBufferAddress());
			break;
	}
}