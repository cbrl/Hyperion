#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "display/viewport.h"


// Rasterizer depth bias values
#define DEPTH_BIAS 50
#define SLOPE_SCALED_DEPTH_BIAS 1.0f
#define DEPTH_BIAS_CLAMP 0.0f


class ShadowMapBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ShadowMapBuffer(ID3D11Device& device,
	                u32 map_count,
	                u32 resolution = 512);

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


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the viewport and DSVs
	void bindViewport(ID3D11DeviceContext& device_context) const {
		viewport.bind(device_context);
	}

	// Bind the raster state for the cube std::map
	void bindRasterState(ID3D11DeviceContext& device_context) const {
		Pipeline::RS::bindState(device_context, raster_state.Get());
	}

	// Bind the depth stencil view for the cube std::map
	void bindDSV(ID3D11DeviceContext& device_context, size_t index) const {
		Pipeline::OM::bindRTVsAndDSV(device_context, 0, nullptr, dsvs[index].Get());
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

	// Get the resolution of the shadow map
	[[nodiscard]]
	u32 getMapRes() const noexcept {
		return viewport.getSize()[0];
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* getSRV() const noexcept {
		return srv.Get();
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* const* getSRVAddress() const noexcept {
		return srv.GetAddressOf();
	}


private:
	Viewport viewport;
	ComPtr<ID3D11RasterizerState> raster_state;

	std::vector<ComPtr<ID3D11DepthStencilView>> dsvs;
	ComPtr<ID3D11ShaderResourceView> srv;
};


class ShadowCubeMapBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ShadowCubeMapBuffer(ID3D11Device& device,
	                    u32 cube_map_count,
	                    u32 resolution = 512);

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

	// Bind the viewport and DSVs
	void bindViewport(ID3D11DeviceContext& device_context) const {
		viewport.bind(device_context);
	}

	// Bind the raster state for the cube std::map
	void bindRasterState(ID3D11DeviceContext& device_context) const {
		Pipeline::RS::bindState(device_context, raster_state.Get());
	}

	// Bind the depth stencil view for the cube std::map
	void bindDSV(ID3D11DeviceContext& device_context, size_t index) const {
		Pipeline::OM::bindRTVAndDSV(device_context, nullptr, dsvs[index].Get());
	}

	// Clear the DSVs
	void clear(ID3D11DeviceContext& device_context) const {
		for (const auto& dsv : dsvs) {
			Pipeline::OM::clearDSV(device_context, dsv.Get());
		}
	}

	// Get the number of individual maps in this buffer
	[[nodiscard]]
	size_t getMapCount() const noexcept {
		return dsvs.size();
	}

	// Get the number of cube maps in this buffer
	[[nodiscard]]
	size_t getCubeMapCount() const noexcept {
		return dsvs.size() / 6;
	}

	// Get the resolution of the shadow map
	[[nodiscard]]
	u32 getMapRes() const noexcept {
		return viewport.getSize()[0];
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* getSRV() const noexcept {
		return srv.Get();
	}

	[[nodiscard]]
	ID3D11ShaderResourceView* const* getSRVAddress() const noexcept {
		return srv.GetAddressOf();
	}


private:
	Viewport viewport;
	ComPtr<ID3D11RasterizerState> raster_state;

	std::vector<ComPtr<ID3D11DepthStencilView>> dsvs;
	ComPtr<ID3D11ShaderResourceView> srv;
};
