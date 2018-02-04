#include "stdafx.h"
#include "RenderStateMgr.h"
#include "rendering\RenderingMgr.h"

const RenderStateMgr* RenderStateMgr::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetRenderStateMgr();
}


RenderStateMgr::RenderStateMgr(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	device(device),
	deviceContext(deviceContext)
{
}


RenderStateMgr::~RenderStateMgr() {
}


void RenderStateMgr::SetupStates() {
	// Blend states
	Opaque();
	AlphaBlend();
	Additive();
	NonPremultiplied();

	// Depth stencil states
	DepthNone();
	DepthDefault();
	DepthRead();

	// Rasterizer states
	CullNone();
	CullClockwise();
	CullCounterClockwise();
	Wireframe();

	// Sampler states
	PointWrap();
	PointClamp();
	LinearWrap();
	LinearClamp();
	AnisotropicWrap();
	AnisotropicClamp();
}


HRESULT RenderStateMgr::CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** pResult) {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend  = desc.RenderTarget[0].SrcBlendAlpha  = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp   = desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	if (SUCCEEDED(hr))
		SetDebugObjectName(*pResult, "RenderStateMgr BlendState");

	return hr;
}


HRESULT RenderStateMgr::CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState** pResult) {
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

	if (SUCCEEDED(hr))
		SetDebugObjectName(*pResult, "RenderStateMgr DepthState");

	return hr;
}


HRESULT RenderStateMgr::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** pResult) {
	D3D11_RASTERIZER_DESC desc = {};

	desc.CullMode          = cullMode;
	desc.FillMode          = fillMode;
	desc.DepthClipEnable   = TRUE;
	desc.MultisampleEnable = TRUE;

	HRESULT hr = device->CreateRasterizerState(&desc, pResult);

	if (SUCCEEDED(hr))
		SetDebugObjectName(*pResult, "RenderStateMgr RasterState");

	return hr;
}


HRESULT RenderStateMgr::CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** pResult) {
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = filter;

	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;

	desc.MaxAnisotropy = (device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;

	desc.MaxLOD         = FLT_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = device->CreateSamplerState(&desc, pResult);

	if (SUCCEEDED(hr))
		SetDebugObjectName(*pResult, "RenderStateMgr SamplerState");

	return hr;
}


//----------------------------------------------------------------------------------
// Create blend states
//----------------------------------------------------------------------------------
void RenderStateMgr::Opaque() {
	if (opaque) {
		return;
	}
	else {
		auto temp = opaque.GetAddressOf();

		DX::ThrowIfFailed(CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, temp), "Error creating opaque blend state");
	}


}


void RenderStateMgr::AlphaBlend() {
	if (alphaBlend) {
		return;
	}
	else {
		auto temp = alphaBlend.GetAddressOf();

		DX::ThrowIfFailed(CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, temp), "Error creating alphablend blend state");
	}
}


void RenderStateMgr::Additive() {
	if (additive) {
		return;
	}
	else {
		auto temp = additive.GetAddressOf();

		DX::ThrowIfFailed(CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, temp), "Error creating additive blend state");
	}
}


void RenderStateMgr::NonPremultiplied() {
	if (nonPremultiplied) {
		return;
	}
	else {
		auto temp = nonPremultiplied.GetAddressOf();

		DX::ThrowIfFailed(CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, temp), "Error creating nonpremultiplied blend state");
	}
}


//----------------------------------------------------------------------------------
// Create depth stencil states
//----------------------------------------------------------------------------------
void RenderStateMgr::DepthNone() {
	if (depthNone) {
		return;
	}
	else {
		auto temp = depthNone.GetAddressOf();

		DX::ThrowIfFailed(CreateDepthStencilState(false, false, temp),
		                  "Error creating depthnone depth state");
	}
}


void RenderStateMgr::DepthDefault() {
	if (depthDefault) {
		return;
	}
	else {
		auto temp = depthDefault.GetAddressOf();

		DX::ThrowIfFailed(CreateDepthStencilState(true, true, temp),
		                  "Error creating depthdefault depth state");
	}
}


void RenderStateMgr::DepthRead() {
	if (depthRead) {
		return;
	}
	else {
		auto temp = depthRead.GetAddressOf();

		DX::ThrowIfFailed(CreateDepthStencilState(true, false, temp),
		                  "Error creating depthread depth state");
	}
}


//----------------------------------------------------------------------------------
// Create rasterizer states
//----------------------------------------------------------------------------------
void RenderStateMgr::CullNone() {
	if (cullNone) {
		return;
	}
	else {
		auto temp = cullNone.GetAddressOf();

		DX::ThrowIfFailed(CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, temp),
		                  "Error creating cullnone raster state");
	}
}


void RenderStateMgr::CullClockwise() {
	if (cullClockwise) {
		return;
	}
	else {
		auto temp = cullClockwise.GetAddressOf();

		DX::ThrowIfFailed(CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, temp),
		                  "Error creating cullclockwise raster state");
	}
}


void RenderStateMgr::CullCounterClockwise() {
	if (cullCounterClockwise) {
		return;
	}
	else {
		auto temp = cullCounterClockwise.GetAddressOf();

		DX::ThrowIfFailed(CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, temp),
		                  "Error creating cullcounterclockwise raster state");
	}
}


void RenderStateMgr::Wireframe() {
	if (wireframe) {
		return;
	}
	else {
		auto temp = wireframe.GetAddressOf();

		DX::ThrowIfFailed(CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, temp),
		                  "Error creating wireframe raster state");
	}
}


//----------------------------------------------------------------------------------
// Create sampler states
//----------------------------------------------------------------------------------
void RenderStateMgr::PointWrap() {
	if (pointWrap) {
		return;
	}
	else {
		auto temp = pointWrap.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, temp),
						  "Error creating pointwrap sampler state");
	}
}


void RenderStateMgr::PointClamp() {
	if (pointClamp) {
		return;
	}
	else {
		auto temp = pointClamp.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, temp),
						  "Error creating pointclamp sampler state");
	}
}


void RenderStateMgr::LinearWrap() {
	if (linearWrap) {
		return;
	}
	else {
		auto temp = linearWrap.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, temp),
						  "Error creating linearwrap sampler state");
	}
}


void RenderStateMgr::LinearClamp() {
	if (linearClamp) {
		return;
	}
	else {
		auto temp = linearClamp.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, temp),
						  "Error creating linearclamp sampler state");
	}
}


void RenderStateMgr::AnisotropicWrap() {
	if (anisotropicWrap) {
		return;
	}
	else {
		auto temp = anisotropicWrap.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, temp),
						  "Error creating anisotropicwrap sampler state");
	}
}


void RenderStateMgr::AnisotropicClamp() {
	if (anisotropicClamp) {
		return;
	}
	else {
		auto temp = anisotropicClamp.GetAddressOf();

		DX::ThrowIfFailed(CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, temp),
						  "Error creating anisotropicclamp sampler state");
	}
}


//----------------------------------------------------------------------------------
// Bind blend states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindOpaque(float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(opaque.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAlphaBlend(float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(alphaBlend.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAdditive(float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(additive.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindNonPremultiplied(float blendFactor[4], unsigned int sampleMask) const {
	deviceContext->OMSetBlendState(nonPremultiplied.Get(), blendFactor, sampleMask);
}


//----------------------------------------------------------------------------------
// Bind depth stencil states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindDepthNone(unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthNone.Get(), stencilRef);
}


void RenderStateMgr::BindDepthDefault(unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthDefault.Get(), stencilRef);
}


void RenderStateMgr::BindDepthRead(unsigned int stencilRef) const {
	deviceContext->OMSetDepthStencilState(depthRead.Get(), stencilRef);
}


//----------------------------------------------------------------------------------
// Bind rasterizer states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindCullNone() const {
	deviceContext->RSSetState(cullNone.Get());
}


void RenderStateMgr::BindCullClockwise() const {
	deviceContext->RSSetState(cullClockwise.Get());
}


void RenderStateMgr::BindCullCounterClockwise() const {
	deviceContext->RSSetState(cullCounterClockwise.Get());
}


void RenderStateMgr::BindWireframe() const {
	deviceContext->RSSetState(wireframe.Get());
}


//----------------------------------------------------------------------------------
// Bind sampler states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindPointWrap() const {
	//deviceContext->CSSetSamplers;
	//deviceContext->DSSetSamplers;
	//deviceContext->GSGetSamplers;
	//deviceContext->HSSetSamplers;
	//deviceContext->VSSetSamplers;

	deviceContext->PSSetSamplers(POINT_WRAP_SLOT, 1, pointWrap.GetAddressOf());
}


void RenderStateMgr::BindPointClamp() const {
	deviceContext->PSSetSamplers(POINT_CLAMP_SLOT, 1, pointClamp.GetAddressOf());
}


void RenderStateMgr::BindLinearWrap() const {
	deviceContext->PSSetSamplers(LINEAR_WRAP_SLOT, 1, linearWrap.GetAddressOf());
}


void RenderStateMgr::BindLinearClamp() const {
	deviceContext->PSSetSamplers(LINEAR_CLAMP_SLOT, 1, linearClamp.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicWrap() const {
	deviceContext->PSSetSamplers(ANISO_WRAP_SLOT, 1, anisotropicWrap.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicClamp() const {
	deviceContext->PSSetSamplers(ANISO_CLAMP_SLOT, 1, anisotropicClamp.GetAddressOf());
}
