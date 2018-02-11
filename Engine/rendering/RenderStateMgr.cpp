#include "stdafx.h"
#include "RenderStateMgr.h"
#include "rendering\RenderingMgr.h"


const RenderStateMgr* RenderStateMgr::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetRenderStateMgr();
}


RenderStateMgr::RenderStateMgr() {
	SetupStates(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());
}


RenderStateMgr::~RenderStateMgr() {
}


void RenderStateMgr::SetupStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	// Blend states
	CreateBlendStates(device);

	// Depth stencil states
	CreateDepthStencilStates(device);

	// Rasterizer states
	CreateRasterizerStates(device);

	// Sampler states
	CreateSamplerStates(device);

	// Bind Sampler states
	BindLinearClamp(deviceContext);
	BindLinearWrap(deviceContext);
	BindPointClamp(deviceContext);
	BindPointWrap(deviceContext);
	BindAnisotropicClamp(deviceContext);
	BindAnisotropicWrap(deviceContext);
}


HRESULT RenderStateMgr::CreateBlendState(ID3D11Device* device, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** pResult) {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend  = desc.RenderTarget[0].SrcBlendAlpha  = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp   = desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*pResult, "RenderStateMgr BlendState");
	}

	return hr;
}


HRESULT RenderStateMgr::CreateDepthStencilState(ID3D11Device* device, bool enable, bool writeEnable, ID3D11DepthStencilState** pResult) {
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable    = enable ? TRUE : FALSE;
	desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable    = FALSE;
	desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	desc.BackFace = desc.FrontFace;

	HRESULT hr = device->CreateDepthStencilState(&desc, pResult);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*pResult, "RenderStateMgr DepthState");
	}

	return hr;
}


HRESULT RenderStateMgr::CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** pResult) {
	D3D11_RASTERIZER_DESC desc = {};

	desc.CullMode          = cullMode;
	desc.FillMode          = fillMode;
	desc.DepthClipEnable   = TRUE;
	desc.MultisampleEnable = TRUE;

	HRESULT hr = device->CreateRasterizerState(&desc, pResult);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*pResult, "RenderStateMgr RasterState");
	}

	return hr;
}


HRESULT RenderStateMgr::CreateSamplerState(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** pResult) {
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = filter;

	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;

	desc.MaxAnisotropy = (device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;

	desc.MaxLOD         = FLT_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = device->CreateSamplerState(&desc, pResult);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*pResult, "RenderStateMgr SamplerState");
	}

	return hr;
}


void RenderStateMgr::CreateBlendStates(ID3D11Device* device) {
	// Opaque
	DX::ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, opaque.GetAddressOf()),
					  "Error creating opaque blend state");

	// Alpha Blend
	DX::ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, alphaBlend.GetAddressOf()),
					  "Error creating alphablend blend state");

	// Additive
	DX::ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, additive.GetAddressOf()),
					  "Error creating additive blend state");

	// Non-Premultiplied
	DX::ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, nonPremultiplied.GetAddressOf()),
					  "Error creating nonpremultiplied blend state");
}


void RenderStateMgr::CreateDepthStencilStates(ID3D11Device* device) {
	// Depth None
	DX::ThrowIfFailed(CreateDepthStencilState(device, false, false, depthNone.GetAddressOf()),
					  "Error creating depthnone depth state");

	// Depth Default
	DX::ThrowIfFailed(CreateDepthStencilState(device, true, true, depthDefault.GetAddressOf()),
					  "Error creating depthdefault depth state");

	// Depth Read
	DX::ThrowIfFailed(CreateDepthStencilState(device, true, false, depthRead.GetAddressOf()),
					  "Error creating depthread depth state");
}


void RenderStateMgr::CreateRasterizerStates(ID3D11Device* device) {
	// Cull None
	DX::ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_SOLID, cullNone.GetAddressOf()),
					  "Error creating cullnone raster state");

	// Cull Clockwise
	DX::ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_FRONT, D3D11_FILL_SOLID, cullClockwise.GetAddressOf()),
					  "Error creating cullclockwise raster state");

	// Cull Counterclockwise
	DX::ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_BACK, D3D11_FILL_SOLID, cullCounterClockwise.GetAddressOf()),
					  "Error creating cullcounterclockwise raster state");

	// Wireframe
	DX::ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, wireframe.GetAddressOf()),
					  "Error creating wireframe raster state");
}


void RenderStateMgr::CreateSamplerStates(ID3D11Device* device) {
	// Point Wrap
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pointWrap.GetAddressOf()),
					  "Error creating pointwrap sampler state");

	// Point Clamp
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pointClamp.GetAddressOf()),
					  "Error creating pointclamp sampler state");

	// Linear Wrap
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linearWrap.GetAddressOf()),
					  "Error creating linearwrap sampler state");

	// Linear Clamp
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, linearClamp.GetAddressOf()),
					  "Error creating linearclamp sampler state");

	// Anisotropic Wrap
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, anisotropicWrap.GetAddressOf()),
					  "Error creating anisotropicwrap sampler state");

	// Anisotropic Clamp
	DX::ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, anisotropicClamp.GetAddressOf()),
					  "Error creating anisotropicclamp sampler state");
}


//----------------------------------------------------------------------------------
// Bind blend states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindOpaque(ID3D11DeviceContext* deviceContext, float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(opaque.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAlphaBlend(ID3D11DeviceContext* deviceContext, float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(alphaBlend.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAdditive(ID3D11DeviceContext* deviceContext, float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(additive.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindNonPremultiplied(ID3D11DeviceContext* deviceContext, float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(nonPremultiplied.Get(), blendFactor, sampleMask);
}


//----------------------------------------------------------------------------------
// Bind depth stencil states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindDepthNone(ID3D11DeviceContext* deviceContext, unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthNone.Get(), stencilRef);
}


void RenderStateMgr::BindDepthDefault(ID3D11DeviceContext* deviceContext, unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthDefault.Get(), stencilRef);
}


void RenderStateMgr::BindDepthRead(ID3D11DeviceContext* deviceContext, unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthRead.Get(), stencilRef);
}


//----------------------------------------------------------------------------------
// Bind rasterizer states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindCullNone(ID3D11DeviceContext* deviceContext) const {
	deviceContext->RSSetState(cullNone.Get());
}


void RenderStateMgr::BindCullClockwise(ID3D11DeviceContext* deviceContext) const {
	deviceContext->RSSetState(cullClockwise.Get());
}


void RenderStateMgr::BindCullCounterClockwise(ID3D11DeviceContext* deviceContext) const {
	deviceContext->RSSetState(cullCounterClockwise.Get());
}


void RenderStateMgr::BindWireframe(ID3D11DeviceContext* deviceContext) const {
	deviceContext->RSSetState(wireframe.Get());
}


//----------------------------------------------------------------------------------
// Bind sampler states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindPointWrap(ID3D11DeviceContext* deviceContext) const {
	//deviceContext->CSSetSamplers;
	//deviceContext->DSSetSamplers;
	//deviceContext->GSGetSamplers;
	//deviceContext->HSSetSamplers;
	//deviceContext->VSSetSamplers;

	deviceContext->PSSetSamplers(POINT_WRAP_SLOT, 1, pointWrap.GetAddressOf());
}


void RenderStateMgr::BindPointClamp(ID3D11DeviceContext* deviceContext) const {
	deviceContext->PSSetSamplers(POINT_CLAMP_SLOT, 1, pointClamp.GetAddressOf());
}


void RenderStateMgr::BindLinearWrap(ID3D11DeviceContext* deviceContext) const {
	deviceContext->PSSetSamplers(LINEAR_WRAP_SLOT, 1, linearWrap.GetAddressOf());
}


void RenderStateMgr::BindLinearClamp(ID3D11DeviceContext* deviceContext) const {
	deviceContext->PSSetSamplers(LINEAR_CLAMP_SLOT, 1, linearClamp.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicWrap(ID3D11DeviceContext* deviceContext) const {
	deviceContext->PSSetSamplers(ANISO_WRAP_SLOT, 1, anisotropicWrap.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicClamp(ID3D11DeviceContext* deviceContext) const {
	deviceContext->PSSetSamplers(ANISO_CLAMP_SLOT, 1, anisotropicClamp.GetAddressOf());
}
