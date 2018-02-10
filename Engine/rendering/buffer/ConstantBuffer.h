#pragma once
#include "stdafx.h"

#include <memory.h>
#include <wrl\client.h>
#include <d3d11.h>
#include "util\EngineUtil.h"

using Microsoft::WRL::ComPtr;

template<typename DataT>
struct ConstantBuffer {
	public:
		ConstantBuffer(ID3D11Device* device) {
			CreateBuffer(device);
		}
		~ConstantBuffer() = default;

		void CreateBuffer(ID3D11Device* device) {
			D3D11_BUFFER_DESC bufferDesc = {};

			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = sizeof(DataT);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, buffer.ReleaseAndGetAddressOf()), "Failed to create buffer");
		}

		ID3D11Buffer** GetBufferAddress() { return buffer.GetAddressOf(); }

		void UpdateData(ID3D11DeviceContext* deviceContext, const DataT& data) {
			D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};

			DX::ThrowIfFailed(deviceContext->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedBuffer), "Failed to map buffer");

			memcpy(mappedBuffer.pData, &data, sizeof(DataT));

			deviceContext->Unmap(buffer.Get(), NULL);
		}


	private:
		ComPtr<ID3D11Buffer> buffer;
};