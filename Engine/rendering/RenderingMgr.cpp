#include "stdafx.h"
#include "RenderingMgr.h"


RenderingMgr::RenderingMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	hWnd(hWnd),
	device(device),
	deviceContext(deviceContext)
{
}


RenderingMgr::~RenderingMgr() {
}


bool RenderingMgr::Init() {
	// Create buffer manager
	cBufferMgr = make_unique<CBufferMgr>(device.Get(), deviceContext);


	// Create sampler state
	sampler = make_unique<Sampler>(device.Get());


	// Create shader manager
	shaderMgr = make_unique<ShaderMgr>();
	if (!shaderMgr->Init(hWnd, device.Get())) {
		return false;
	}

	return true;
}

void RenderingMgr::BindShader(ShaderTypes shader) {
	switch (shader) {
		case ShaderTypes::ColorShader:
			shaderMgr->BindShader(ShaderTypes::ColorShader, deviceContext.Get());
			cBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			break;

		case ShaderTypes::TextureShader:
			shaderMgr->BindShader(ShaderTypes::TextureShader, deviceContext.Get());
			cBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			cBufferMgr->BindCBuffer(BufferTypes::CameraBuffer);
			deviceContext->PSSetSamplers(0, 1, sampler->samplerState.GetAddressOf());
			break;

		case ShaderTypes::LightShader:
			shaderMgr->BindShader(ShaderTypes::LightShader, deviceContext.Get());
			cBufferMgr->BindCBuffer(BufferTypes::MatrixBuffer);
			cBufferMgr->BindCBuffer(BufferTypes::CameraBuffer);
			cBufferMgr->BindCBuffer(BufferTypes::LightBuffer);
			deviceContext->PSSetSamplers(0, 1, sampler->samplerState.GetAddressOf());
			break;

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}
