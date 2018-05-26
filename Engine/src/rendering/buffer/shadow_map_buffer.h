#pragma once

#include <d3d11.h>
#include "datatypes\datatypes.h"
#include "rendering\pipeline.h"


// Rasterizer depth bias values
#define DEPTH_BIAS 50
#define SLOPE_SCALED_DEPTH_BIAS 1.0f
#define DEPTH_BIAS_CLAMP 0.0f


class ShadowMapBuffer final {
	public:
		ShadowMapBuffer(ID3D11Device& device,
						u32 map_count,
						u32 width = 512,
						u32 height = 512);

		~ShadowMapBuffer() = default;

		// Bind the viewport and DSVs
		void BindViewport(ID3D11DeviceContext& device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		// Bind the raster state for the cube map
		void BindRasterState(ID3D11DeviceContext& device_context) const {
			Pipeline::RS::BindState(device_context, raster_state.Get());
		}

		// Bind the depth stencil view for the cube map
		void BindDSV(ID3D11DeviceContext& device_context, size_t index) const {
			Pipeline::OM::BindRTVs(device_context, 1, nullptr, dsvs[index].Get());
		}

		// Clear the DSVs
		void Clear(ID3D11DeviceContext& device_context) const {
			for (const auto& dsv : dsvs) {
				Pipeline::OM::ClearDSV(device_context, dsv.Get());
			}
		}

		// Get the number of shadow maps in this buffer
		size_t GetMapCount() const {
			return dsvs.size();
		}

		ID3D11ShaderResourceView& GetSRV() const {
			return *srv.Get();
		}

		ID3D11ShaderResourceView* const* GetSRVAddress() const {
			return srv.GetAddressOf();
		}


	private:
		D3D11_VIEWPORT viewport;
		ComPtr<ID3D11RasterizerState> raster_state;

		vector<ComPtr<ID3D11DepthStencilView>> dsvs;
		ComPtr<ID3D11ShaderResourceView> srv;
};



class ShadowCubeMapBuffer final {
	public:
		ShadowCubeMapBuffer(ID3D11Device& device,
							u32 cube_map_count,
							u32 width = 512,
							u32 height = 512);

		~ShadowCubeMapBuffer() = default;

		// Bind the viewport and DSVs
		void BindViewport(ID3D11DeviceContext& device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		// Bind the raster state for the cube map
		void BindRasterState(ID3D11DeviceContext& device_context) const {
			Pipeline::RS::BindState(device_context, raster_state.Get());
		}

		// Bind the depth stencil view for the cube map
		void BindDSV(ID3D11DeviceContext& device_context, size_t index) const {
			Pipeline::OM::BindRTVs(device_context, 1, nullptr, dsvs[index].Get());
		}

		// Clear the DSVs
		void Clear(ID3D11DeviceContext& device_context) const {
			for (const auto& dsv : dsvs) {
				Pipeline::OM::ClearDSV(device_context, dsv.Get());
			}
		}

		// Get the number of individual maps in this buffer
		size_t GetMapCount() const {
			return dsvs.size();
		}

		// Get the number of cube maps in this buffer
		size_t GetCubeMapCount() const {
			return dsvs.size() / 6;
		}

		ID3D11ShaderResourceView& GetSRV() const {
			return *srv.Get();
		}

		ID3D11ShaderResourceView* const* GetSRVAddress() const {
			return srv.GetAddressOf();
		}


	private:
		D3D11_VIEWPORT viewport;
		ComPtr<ID3D11RasterizerState> raster_state;

		vector<ComPtr<ID3D11DepthStencilView>> dsvs;
		ComPtr<ID3D11ShaderResourceView> srv;
};