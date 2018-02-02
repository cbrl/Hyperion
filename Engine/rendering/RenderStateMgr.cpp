#include "stdafx.h"
#include "RenderStateMgr.h"


RenderStateMgr::RenderStateMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	hWnd(hWnd),
	device(device),
	deviceContext(deviceContext)
{
}


RenderStateMgr::~RenderStateMgr() {
}


bool RenderStateMgr::Init() {
	// Create shader manager
	shaderMgr = make_unique<ShaderMgr>();
	if (!shaderMgr->Init(hWnd, device.Get())) {
		return false;
	}


	// Create sampler state
	sampler = make_unique<Sampler>(device.Get());

	// Bind sampler
	deviceContext->PSSetSamplers(0, 1, sampler->samplerState.GetAddressOf());


	return true;
}

void RenderStateMgr::BindShader(ShaderTypes shader) {
	shaderMgr->BindShader(shader, deviceContext.Get());
}
