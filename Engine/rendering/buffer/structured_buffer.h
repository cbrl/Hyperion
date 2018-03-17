#pragma once

#include <d3d11.h>
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


template<typename DataT>
class StructuredBuffer {
	public:
		StructuredBuffer(ID3D11Device* device, u32 size) : size(size) {
			CreateBuffer(device);
		}

		~StructuredBuffer() = default;


		void UpdateData(ID3D11Device* device, ID3D11DeviceContext* device_context, const vector<DataT>& data) {
			if (data.size() == 0) {
				return;
			}

			// Recreate the buffer if the array of data being fed to it is larger than the buffer
			if (data.size() > size) {
				size = static_cast<u32>(data.size());
				CreateBuffer(device);
			}

			// Map the buffer and update the data
			D3D11_MAPPED_SUBRESOURCE mapped_data = {};

			ThrowIfFailed(device_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
			              "Failed to map structured buffer");

			memcpy(mapped_data.pData, data.data(), sizeof(DataT) * size);

			device_context->Unmap(buffer.Get(), NULL);
		}

		// Bind the buffer to the specified pipeline stage
		template<typename StageT>
		void Bind(ID3D11DeviceContext* device_context, u32 slot) {
			StageT::BindSRVs(device_context, slot, 1, srv.GetAddressOf());
		}


	private:
		void CreateBuffer(ID3D11Device* device) {
			D3D11_BUFFER_DESC desc = {};

			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(DataT) * size;
			desc.StructureByteStride = sizeof(DataT);

			ThrowIfFailed(device->CreateBuffer(&desc, nullptr, buffer.ReleaseAndGetAddressOf()),
						  "Failed to create structured buffer");

			SetDebugObjectName(buffer.Get(), "Structured Buffer");


			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
			srv_desc.Buffer.FirstElement = 0;
			srv_desc.Buffer.NumElements = size;
			srv_desc.Format = DXGI_FORMAT_UNKNOWN;
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

			ThrowIfFailed(device->CreateShaderResourceView(buffer.Get(), &srv_desc, srv.ReleaseAndGetAddressOf()),
						  "Failed to create structured buffer SRV");

			SetDebugObjectName(srv.Get(), "Structured Buffer SRV");
		}


	private:
		ComPtr<ID3D11Buffer> buffer;
		ComPtr<ID3D11ShaderResourceView> srv;

		// Number of elements in the buffer
		u32 size;
};