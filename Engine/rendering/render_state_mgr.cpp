#include "stdafx.h"
#include "render_state_mgr.h"


RenderStateMgr::RenderStateMgr(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	SetupStates(device, device_context);
}


RenderStateMgr::~RenderStateMgr() {
}


void RenderStateMgr::SetupStates(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	// Blend states
	CreateBlendStates(device);

	// Depth stencil states
	CreateDepthStencilStates(device);

	// Rasterizer states
	CreateRasterizerStates(device);

	// Sampler states
	CreateSamplerStates(device);

	// Bind Sampler states
	BindLinearClamp(device_context);
	BindLinearWrap(device_context);
	BindPointClamp(device_context);
	BindPointWrap(device_context);
	BindAnisotropicClamp(device_context);
	BindAnisotropicWrap(device_context);
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
	ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, opaque.GetAddressOf()),
					  "Error creating opaque blend state");

	// Alpha Blend
	ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, alpha_blend.GetAddressOf()),
					  "Error creating alphablend blend state");

	// Additive
	ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, additive.GetAddressOf()),
					  "Error creating additive blend state");

	// Non-Premultiplied
	ThrowIfFailed(CreateBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, non_premultiplied.GetAddressOf()),
					  "Error creating nonpremultiplied blend state");
}


void RenderStateMgr::CreateDepthStencilStates(ID3D11Device* device) {
	// Depth None
	ThrowIfFailed(CreateDepthStencilState(device, false, false, depth_none.GetAddressOf()),
					  "Error creating depthnone depth state");

	// Depth Default
	ThrowIfFailed(CreateDepthStencilState(device, true, true, depth_default.GetAddressOf()),
					  "Error creating depthdefault depth state");

	// Depth Read
	ThrowIfFailed(CreateDepthStencilState(device, true, false, depth_read.GetAddressOf()),
					  "Error creating depthread depth state");
}


void RenderStateMgr::CreateRasterizerStates(ID3D11Device* device) {
	// Cull None
	ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_SOLID, cull_none.GetAddressOf()),
					  "Error creating cullnone raster state");

	// Cull Clockwise
	ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_FRONT, D3D11_FILL_SOLID, cull_clockwise.GetAddressOf()),
					  "Error creating cullclockwise raster state");

	// Cull Counterclockwise
	ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_BACK, D3D11_FILL_SOLID, cull_counter_clockwise.GetAddressOf()),
					  "Error creating cullcounterclockwise raster state");

	// Wireframe
	ThrowIfFailed(CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, wireframe.GetAddressOf()),
					  "Error creating wireframe raster state");
}


void RenderStateMgr::CreateSamplerStates(ID3D11Device* device) {
	// Point Wrap
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, point_wrap.GetAddressOf()),
					  "Error creating pointwrap sampler state");

	// Point Clamp
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, point_clamp.GetAddressOf()),
					  "Error creating pointclamp sampler state");

	// Linear Wrap
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, linear_wrap.GetAddressOf()),
					  "Error creating linearwrap sampler state");

	// Linear Clamp
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, linear_clamp.GetAddressOf()),
					  "Error creating linearclamp sampler state");

	// Anisotropic Wrap
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, anisotropic_wrap.GetAddressOf()),
					  "Error creating anisotropicwrap sampler state");

	// Anisotropic Clamp
	ThrowIfFailed(CreateSamplerState(device, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, anisotropic_clamp.GetAddressOf()),
					  "Error creating anisotropicclamp sampler state");
}


//----------------------------------------------------------------------------------
// Bind blend states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindOpaque(ID3D11DeviceContext* device_context, float blendFactor[4], u32 sampleMask) const {
	Pipeline::OM::BindBlendState(device_context, opaque.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAlphaBlend(ID3D11DeviceContext* device_context, float blendFactor[4], u32 sampleMask) const {
	Pipeline::OM::BindBlendState(device_context, alpha_blend.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindAdditive(ID3D11DeviceContext* device_context, float blendFactor[4], u32 sampleMask) const {
	Pipeline::OM::BindBlendState(device_context, additive.Get(), blendFactor, sampleMask);
}


void RenderStateMgr::BindNonPremultiplied(ID3D11DeviceContext* device_context, float blendFactor[4], u32 sampleMask) const {
	Pipeline::OM::BindBlendState(device_context, non_premultiplied.Get(), blendFactor, sampleMask);
}


//----------------------------------------------------------------------------------
// Bind depth stencil states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindDepthNone(ID3D11DeviceContext* device_context, u32 stencilRef) const {
	Pipeline::OM::BindDepthStencilState(device_context, depth_none.Get(), stencilRef);
}


void RenderStateMgr::BindDepthDefault(ID3D11DeviceContext* device_context, u32 stencilRef) const {
	Pipeline::OM::BindDepthStencilState(device_context, depth_default.Get(), stencilRef);
}


void RenderStateMgr::BindDepthRead(ID3D11DeviceContext* device_context, u32 stencilRef) const {
	Pipeline::OM::BindDepthStencilState(device_context, depth_read.Get(), stencilRef);
}


//----------------------------------------------------------------------------------
// Bind rasterizer states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindCullNone(ID3D11DeviceContext* device_context) const {
	Pipeline::RS::BindState(device_context, cull_none.Get());
}


void RenderStateMgr::BindCullClockwise(ID3D11DeviceContext* device_context) const {
	Pipeline::RS::BindState(device_context, cull_clockwise.Get());
}


void RenderStateMgr::BindCullCounterClockwise(ID3D11DeviceContext* device_context) const {
	Pipeline::RS::BindState(device_context, cull_counter_clockwise.Get());
}


void RenderStateMgr::BindWireframe(ID3D11DeviceContext* device_context) const {
	Pipeline::RS::BindState(device_context, wireframe.Get());
}


//----------------------------------------------------------------------------------
// Bind sampler states
//----------------------------------------------------------------------------------
void RenderStateMgr::BindPointWrap(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_POINT_WRAP, 1, point_wrap.GetAddressOf());
}


void RenderStateMgr::BindPointClamp(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_POINT_CLAMP, 1, point_clamp.GetAddressOf());
}


void RenderStateMgr::BindLinearWrap(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_LINEAR_WRAP, 1, linear_wrap.GetAddressOf());
}


void RenderStateMgr::BindLinearClamp(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_LINEAR_CLAMP, 1, linear_clamp.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicWrap(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_ANISO_WRAP, 1, anisotropic_wrap.GetAddressOf());
}


void RenderStateMgr::BindAnisotropicClamp(ID3D11DeviceContext* device_context) const {
	Pipeline::BindSamplers(device_context, SLOT_SAMPLER_ANISO_CLAMP, 1, anisotropic_clamp.GetAddressOf());
}
