#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


class ShadowMapBuffer final {
	public:
		ShadowMapBuffer(ID3D11Device* device, u32 width = 256, u32 height = 256);
		~ShadowMapBuffer() = default;

		void Bind(ID3D11DeviceContext* device_context) const {
			BindViewport(device_context);
			BindDSV(device_context);
		}

		const ID3D11ShaderResourceView* GetSRV() const {
			return srv.Get();
		}


	private:
		void BindViewport(ID3D11DeviceContext* device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		void BindDSV(ID3D11DeviceContext* device_context) const {
			Pipeline::OM::BindRTVs(device_context, 1, nullptr, dsv.Get());
		}


	private:
		ComPtr<ID3D11DepthStencilView>   dsv;
		ComPtr<ID3D11ShaderResourceView> srv;

		D3D11_VIEWPORT viewport;
};