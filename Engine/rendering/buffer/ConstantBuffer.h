#pragma once
#include "stdafx.h"

#include <d3d11.h>
#include <wrl\client.h>
#include <memory.h>
#include "util\EngineUtil.h"

using Microsoft::WRL::ComPtr;

template<typename DataT>
class ConstantBuffer {
	public:
		ConstantBuffer(const ComPtr<ID3D11Device>& device);
		~ConstantBuffer() = default;

		void CreateBuffer(const ComPtr<ID3D11Device>& device);
		void UpdateData(const ComPtr<ID3D11DeviceContext>& deviceContext, const DataT& data);


	public:
		ComPtr<ID3D11Buffer> buffer;
};


template<typename DataT>
ConstantBuffer<DataT>::ConstantBuffer(const ComPtr<ID3D11Device>& device) {
	CreateBuffer(device);
}


template<typename DataT>
void ConstantBuffer<DataT>::CreateBuffer(const ComPtr<ID3D11Device>& device) {
	D3D11_BUFFER_DESC bufferDesc = { };

	bufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth           = sizeof(DataT);
	bufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = 0;

	HR(device->CreateBuffer(&bufferDesc, nullptr, buffer.ReleaseAndGetAddressOf()));
}


template<typename DataT>
void ConstantBuffer<DataT>::UpdateData(const ComPtr<ID3D11DeviceContext>& deviceContext, const DataT& data) {
	D3D11_MAPPED_SUBRESOURCE mappedBuffer = { };

	HR(deviceContext->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedBuffer));

	memcpy(mappedBuffer.pData, &data, sizeof(DataT));

	deviceContext->Unmap(buffer.Get(), NULL);
}