#pragma once

#include <vector>
#include <wrl\client.h>
#include <d3d11.h>
#include "util\engine_util.h"

using std::vector;
using Microsoft::WRL::ComPtr;


template<typename DataT>
class StructuredBuffer {
	public:
		StructuredBuffer(ID3D11Device* device, unsigned int size) : size(size) {
			CreateBuffer(device);
		}
		~StructuredBuffer() = default;

		void UpdateData(ID3D11Device* device, ID3D11DeviceContext* device_context, const vector<DataT>& data) {
			// Recreate the buffer if the data being fed to it is larger than the buffer
			if (data.size() > size) {
				size = static_cast<uint32_t>(data.size());
				CreateBuffer(device);
			}

			// Map the buffer and update the data
			D3D11_MAPPED_SUBRESOURCE mapped_data = {};

			DX::ThrowIfFailed(device_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
			                  "Failed to map structured buffer");

			memcpy(mapped_data.pData, data.data(), sizeof(DataT) * size);

			device_context->Unmap(buffer.Get(), NULL);
		}

		ID3D11Buffer* const* GetBufferAddress() const { return buffer.GetAddressOf(); }
		ID3D11ShaderResourceView* const* GetSRVAddress() const { return srv.GetAddressOf(); }


	private:
		void CreateBuffer(ID3D11Device* device) {
			D3D11_BUFFER_DESC desc = {};

			desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.Usage               = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth           = sizeof(DataT) * size;
			desc.StructureByteStride = sizeof(DataT);

			DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, buffer.ReleaseAndGetAddressOf()),
			                  "Failed to create structured buffer");


			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
			srv_desc.Buffer.FirstElement = 0;
			srv_desc.Buffer.NumElements  = size;
			srv_desc.Format              = DXGI_FORMAT_UNKNOWN;
			srv_desc.ViewDimension       = D3D11_SRV_DIMENSION_BUFFER;

			DX::ThrowIfFailed(device->CreateShaderResourceView(buffer.Get(), &srv_desc, srv.ReleaseAndGetAddressOf()),
							  "Failed to create structured buffer SRV");
		}


	private:
		ComPtr<ID3D11Buffer> buffer;
		ComPtr<ID3D11ShaderResourceView> srv;

		// Number of elements in the buffer
		uint32_t size;
};