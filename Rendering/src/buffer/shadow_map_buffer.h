#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "display/viewport.h"


namespace render {

class IShadowMapBuffer {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	IShadowMapBuffer() = default;
	IShadowMapBuffer(const IShadowMapBuffer& buffer) = delete;
	IShadowMapBuffer(IShadowMapBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~IShadowMapBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	IShadowMapBuffer& operator=(const IShadowMapBuffer& buffer) = delete;
	IShadowMapBuffer& operator=(IShadowMapBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the viewport
	void bindViewport(ID3D11DeviceContext& device_context) const {
		viewport.bind(device_context);
	}

	// Bind the raster state for the cubemap
	void bindRasterState(ID3D11DeviceContext& device_context) const {
		Pipeline::RS::bindState(device_context, raster_state.Get());
	}

	// Bind the depth stencil view for the cubemap
	void bindDSV(ID3D11DeviceContext& device_context, size_t index) const {
		Pipeline::OM::bindRTVsAndDSV(device_context, {}, dsvs[index].Get());
	}

	// Clear the DSVs
	void clear(ID3D11DeviceContext& device_context) const {
		for (const auto& dsv : dsvs) {
			Pipeline::OM::clearDSV(device_context, dsv.Get());
		}
	}

	// Get the number of shadow maps in this buffer
	[[nodiscard]]
	size_t getMapCount() const noexcept {
		return dsvs.size();
	}

	[[nodiscard]]
	u32 getMapRes() const noexcept {
		return viewport.getSize()[0];
	}

	[[nodiscard]]
	i32 getDepthBias() const noexcept {
		D3D11_RASTERIZER_DESC desc;
		raster_state->GetDesc(&desc);
		return desc.DepthBias;
	}

	[[nodiscard]]
	f32 getSlopeScaledDepthBias() const noexcept {
		D3D11_RASTERIZER_DESC desc;
		raster_state->GetDesc(&desc);
		return desc.SlopeScaledDepthBias;
	}

	[[nodiscard]]
	f32 getDepthBiasClamp() const noexcept {
		D3D11_RASTERIZER_DESC desc;
		raster_state->GetDesc(&desc);
		return desc.DepthBiasClamp;
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* getSRV() const noexcept {
		return srv.Get();
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* const* getSRVAddress() const noexcept {
		return srv.GetAddressOf();
	}

protected:

	virtual void init(ID3D11Device& device,
	                  u32 map_count,
	                  u32 resolution,
	                  i32 depth_bias,
	                  f32 slope_scaled_depth_bias,
	                  f32 depth_bias_clamp) = 0;

	Viewport viewport;
	ComPtr<ID3D11RasterizerState> raster_state;

	std::vector<ComPtr<ID3D11DepthStencilView>> dsvs;
	ComPtr<ID3D11ShaderResourceView> srv;
};


class ShadowMapBuffer final : public IShadowMapBuffer {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShadowMapBuffer(ID3D11Device& device,
	                u32 map_count,
	                u32 resolution,
	                i32 depth_bias,
	                f32 slope_scaled_depth_bias,
	                f32 depth_bias_clamp);

	ShadowMapBuffer(const ShadowMapBuffer& buffer) = delete;
	ShadowMapBuffer(ShadowMapBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShadowMapBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShadowMapBuffer& operator=(const ShadowMapBuffer& buffer) = delete;
	ShadowMapBuffer& operator=(ShadowMapBuffer&& buffer) noexcept = default;

private:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void init(ID3D11Device& device,
	          u32 map_count,
	          u32 resolution,
	          i32 depth_bias,
	          f32 slope_scaled_depth_bias,
	          f32 depth_bias_clamp) override;
};


class ShadowCubeMapBuffer final : public IShadowMapBuffer {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShadowCubeMapBuffer(ID3D11Device& device,
	                    u32 cube_map_count,
	                    u32 resolution,
	                    i32 depth_bias,
	                    f32 slope_scaled_depth_bias,
	                    f32 depth_bias_clamp);

	ShadowCubeMapBuffer(const ShadowCubeMapBuffer& buffer) = delete;
	ShadowCubeMapBuffer(ShadowCubeMapBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShadowCubeMapBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShadowCubeMapBuffer& operator=(const ShadowCubeMapBuffer& buffer) = delete;
	ShadowCubeMapBuffer& operator=(ShadowCubeMapBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the number of cube maps in this buffer (cubemaps = maps / 6)
	[[nodiscard]]
	size_t getCubeMapCount() const noexcept {
		return dsvs.size() / 6;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void init(ID3D11Device& device,
	          u32 cube_map_count,
	          u32 resolution,
	          i32 depth_bias,
	          f32 slope_scaled_depth_bias,
	          f32 depth_bias_clamp) override;
};

} //namespace render
