#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


class ShadowMapBuffer final {
	public:
		ShadowMapBuffer(ID3D11Device* device,
						u32 map_count,
						u32 width = 256,
						u32 height = 256);

		~ShadowMapBuffer() = default;

		// Bind the viewport and DSVs
		void Bind(ID3D11DeviceContext* device_context) const {
			BindViewport(device_context);
			BindDSVs(device_context);
		}

		// Clear the DSVs
		void Clear(ID3D11DeviceContext* device_context) const {
			for (const auto& dsv : dsvs) {
				Pipeline::OM::ClearDSV(device_context, dsv.Get());
			}
		}

		// Get the number of shadow maps in this buffer
		u32 GetMapCount() const {
			return static_cast<u32>(dsvs.size());
		}

		const ID3D11ShaderResourceView* GetSRV() const {
			return srv.Get();
		}


	private:
		void BindViewport(ID3D11DeviceContext* device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		void BindDSVs(ID3D11DeviceContext* device_context) const {
			for (const auto& dsv : dsvs) {
				Pipeline::OM::BindRTVs(device_context, 1, nullptr, dsv.Get());
			}
		}


	private:
		vector<ComPtr<ID3D11DepthStencilView>> dsvs;

		ComPtr<ID3D11ShaderResourceView> srv;

		D3D11_VIEWPORT viewport;
};