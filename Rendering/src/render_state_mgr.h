#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"


class RenderStateMgr final {
public:
	RenderStateMgr(ID3D11Device& device, ID3D11DeviceContext& device_context);
	~RenderStateMgr() = default;

	void setupStates(ID3D11Device& device, ID3D11DeviceContext& device_context);


	//----------------------------------------------------------------------------------
	// Bind states
	//----------------------------------------------------------------------------------

	// Blend states
	void bindOpaque(ID3D11DeviceContext& device_context,
	                f32 blend_factor[4] = {},
	                u32 sample_mask     = 0xffffffff) const;

	void bindAlphaBlend(ID3D11DeviceContext& device_context,
	                    f32 blend_factor[4] = {},
	                    u32 sample_mask     = 0xffffffff) const;

	void bindAdditive(ID3D11DeviceContext& device_context,
	                  f32 blend_factor[4] = {},
	                  u32 sample_mask     = 0xffffffff) const;

	void bindNonPremultiplied(ID3D11DeviceContext& device_context,
	                          f32 blend_factor[4] = {},
	                          u32 sample_mask     = 0xffffffff) const;

	// Depth stencil states
	void bindDepthNone(ID3D11DeviceContext& device_context, u32 stencil_ref = 0) const;
	void bindDepthLessEqRW(ID3D11DeviceContext& device_context, u32 stencil_ref = 0) const;
	void bindDepthGreaterEqRW(ID3D11DeviceContext& device_context, u32 stencil_ref = 0) const;
	void bindDepthLessEqRead(ID3D11DeviceContext& device_context, u32 stencil_ref = 0) const;

	// Rasterizer states
	void bindCullNone(ID3D11DeviceContext& device_context) const;
	void bindCullClockwise(ID3D11DeviceContext& device_context) const;
	void bindCullCounterClockwise(ID3D11DeviceContext& device_context) const;
	void bindWireframe(ID3D11DeviceContext& device_context) const;

	// Sampler states
	void bindPointWrap(ID3D11DeviceContext& device_context) const;
	void bindPointClamp(ID3D11DeviceContext& device_context) const;
	void bindLinearWrap(ID3D11DeviceContext& device_context) const;
	void bindLinearClamp(ID3D11DeviceContext& device_context) const;
	void bindAnisotropicWrap(ID3D11DeviceContext& device_context) const;
	void bindAnisotropicClamp(ID3D11DeviceContext& device_context) const;
	void bindPcfSampler(ID3D11DeviceContext& device_context) const;


private:
	//----------------------------------------------------------------------------------
	// Create states
	//----------------------------------------------------------------------------------

	HRESULT createBlendState(ID3D11Device& device,
	                         D3D11_BLEND src_blend,
	                         D3D11_BLEND dest_blend,
	                         _Out_ ID3D11BlendState** p_result) const;

	HRESULT createDepthStencilState(ID3D11Device& device,
	                                bool enable,
	                                bool write_enable,
	                                bool depth_greater,
	                                _Out_ ID3D11DepthStencilState** p_result) const;

	HRESULT createRasterizerState(ID3D11Device& device,
	                              D3D11_CULL_MODE cull_mode,
	                              D3D11_FILL_MODE fill_mode,
	                              _Out_ ID3D11RasterizerState** p_result) const;

	HRESULT createSamplerState(ID3D11Device& device,
	                           D3D11_FILTER filter,
	                           D3D11_TEXTURE_ADDRESS_MODE address_mode,
	                           _Out_ ID3D11SamplerState** p_result) const;


	// Create the standard render states
	void createBlendStates(ID3D11Device& device);
	void createDepthStencilStates(ID3D11Device& device);
	void createRasterizerStates(ID3D11Device& device);
	void createSamplerStates(ID3D11Device& device);


private:
	// Blend states
	ComPtr<ID3D11BlendState> opaque;
	ComPtr<ID3D11BlendState> alpha_blend;
	ComPtr<ID3D11BlendState> additive;
	ComPtr<ID3D11BlendState> non_premultiplied;

	// Depth stencil states
	ComPtr<ID3D11DepthStencilState> depth_none;
	ComPtr<ID3D11DepthStencilState> depth_less_equal_read;
	ComPtr<ID3D11DepthStencilState> depth_less_equal_readwrite;
	ComPtr<ID3D11DepthStencilState> depth_greater_equal_readwrite;

	// Rasterizer states
	ComPtr<ID3D11RasterizerState> cull_none;
	ComPtr<ID3D11RasterizerState> cull_clockwise;
	ComPtr<ID3D11RasterizerState> cull_counter_clockwise;
	ComPtr<ID3D11RasterizerState> wireframe;

	// Sampler states
	ComPtr<ID3D11SamplerState> point_wrap;
	ComPtr<ID3D11SamplerState> point_clamp;
	ComPtr<ID3D11SamplerState> linear_wrap;
	ComPtr<ID3D11SamplerState> linear_clamp;
	ComPtr<ID3D11SamplerState> anisotropic_wrap;
	ComPtr<ID3D11SamplerState> anisotropic_clamp;
	ComPtr<ID3D11SamplerState> pcf_sampler;
};
