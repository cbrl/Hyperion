#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"


enum class BlendStates : u8 {
	Opaque,
	AlphaBlend,
	Additive,
	NonPremultiplied,
	StateCount
};

enum class DepthStencilStates : u8 {
	None,
	LessEqR,
	LessEqRW,
	GreaterEqR,
	GreaterEqRW,
	StateCount
};

enum class RasterStates : u8 {
	CullNone,
	CullClockwise,
	CullCounterClockwise,
	Wireframe,
	StateCount
};

enum class SamplerStates : u8 {
	PointClamp,
	PointWrap,
	LinearClamp,
	LinearWrap,
	AnisotropicClamp,
	AnisotropicWrap,
	PCF,
	StateCount
};


class RenderStateMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	RenderStateMgr(ID3D11Device& device, ID3D11DeviceContext& device_context);
	RenderStateMgr(const RenderStateMgr& mgr) = delete;
	RenderStateMgr(RenderStateMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~RenderStateMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	RenderStateMgr& operator=(const RenderStateMgr& mgr) = delete;
	RenderStateMgr& operator=(RenderStateMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Bind states
	//----------------------------------------------------------------------------------

	void bind(ID3D11DeviceContext& device_context,
	          BlendStates state,
	          f32 blend_factor[4] = {},
	          u32 sample_mask = 0xFFFFFFFF) const;

	void bind(ID3D11DeviceContext& device_context, DepthStencilStates state, u32 stencil_ref = 0) const;

	void bind(ID3D11DeviceContext& device_context, RasterStates state) const;

	void bind(ID3D11DeviceContext& device_context, SamplerStates state, u32 slot) const;


private:
	//----------------------------------------------------------------------------------
	// Create states
	//----------------------------------------------------------------------------------

	void setupStates(ID3D11Device& device, ID3D11DeviceContext& device_context);

	// Create the standard render states
	void createBlendStates(ID3D11Device& device);
	void createDepthStencilStates(ID3D11Device& device);
	void createRasterizerStates(ID3D11Device& device);
	void createSamplerStates(ID3D11Device& device);

	HRESULT createBlendState(ID3D11Device& device,
	                         D3D11_BLEND src_blend,
	                         D3D11_BLEND dest_blend,
	                         _Out_ gsl::not_null<ID3D11BlendState**> p_result) const;

	HRESULT createDepthStencilState(ID3D11Device& device,
	                                bool enable,
	                                bool write_enable,
	                                bool depth_greater,
	                                _Out_ gsl::not_null<ID3D11DepthStencilState**> p_result) const;

	HRESULT createRasterizerState(ID3D11Device& device,
	                              D3D11_CULL_MODE cull_mode,
	                              D3D11_FILL_MODE fill_mode,
	                              _Out_ gsl::not_null<ID3D11RasterizerState**> p_result) const;

	HRESULT createSamplerState(ID3D11Device& device,
	                           D3D11_FILTER filter,
	                           D3D11_TEXTURE_ADDRESS_MODE address_mode,
	                           _Out_ gsl::not_null<ID3D11SamplerState**> p_result) const;


	//----------------------------------------------------------------------------------
	// Member Fucntions - Get
	//----------------------------------------------------------------------------------

	ID3D11BlendState* get(BlendStates state) const {
		return blend_states[static_cast<u8>(state)].Get();
	}
	ID3D11DepthStencilState* get(DepthStencilStates state) const {
		return depth_states[static_cast<u8>(state)].Get();
	}
	ID3D11RasterizerState* get(RasterStates state) const {
		return raster_states[static_cast<u8>(state)].Get();
	}
	ID3D11SamplerState* get(SamplerStates state) const {
		return sampler_states[static_cast<u8>(state)].Get();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Get Address Of
	//----------------------------------------------------------------------------------

	// Blend States
	gsl::not_null<ID3D11BlendState**> getAddressOf(BlendStates state) {

		auto& state_ptr = blend_states[static_cast<u8>(state)];
		return gsl::not_null<ID3D11BlendState**>(state_ptr.GetAddressOf());
	}

	// Depth States
	gsl::not_null<ID3D11DepthStencilState**> getAddressOf(DepthStencilStates state) {

		auto& state_ptr = depth_states[static_cast<u8>(state)];
		return gsl::not_null<ID3D11DepthStencilState**>(state_ptr.GetAddressOf());
	}

	// Raster Staes
	gsl::not_null<ID3D11RasterizerState**> getAddressOf(RasterStates state) {
		
		auto& state_ptr = raster_states[static_cast<u8>(state)];
		return gsl::not_null<ID3D11RasterizerState**>(state_ptr.GetAddressOf());
	}

	// Sampler States
	gsl::not_null<ID3D11SamplerState**> getAddressOf(SamplerStates state) {

		auto& state_ptr = sampler_states[static_cast<u8>(state)];
		return gsl::not_null<ID3D11SamplerState**>(state_ptr.GetAddressOf());
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	
	ComPtr<ID3D11BlendState>        blend_states[static_cast<u8>(BlendStates::StateCount)];
	ComPtr<ID3D11DepthStencilState> depth_states[static_cast<u8>(DepthStencilStates::StateCount)];
	ComPtr<ID3D11RasterizerState>   raster_states[static_cast<u8>(RasterStates::StateCount)];
	ComPtr<ID3D11SamplerState>      sampler_states[static_cast<u8>(SamplerStates::StateCount)];
};
