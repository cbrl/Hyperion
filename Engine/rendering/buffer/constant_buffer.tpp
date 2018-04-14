template<typename DataT>
ConstantBuffer<DataT>::ConstantBuffer(ID3D11Device* device) {
	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(DataT);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = NULL;

	ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, buffer.ReleaseAndGetAddressOf()),
				  "Failed to create constant buffer");

	SetDebugObjectName(buffer.Get(), "Constant Buffer");
}


template<typename DataT>
void ConstantBuffer<DataT>::UpdateData(ID3D11DeviceContext* device_context, const DataT& data) const {
	D3D11_MAPPED_SUBRESOURCE mapped_data = {};

	ThrowIfFailed(device_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
				  "Failed to map constant buffer");

	memcpy(mapped_data.pData, &data, sizeof(DataT));
	//*(DataT*)mapped_data.pData = data;

	device_context->Unmap(buffer.Get(), NULL);
}


template<typename DataT>
template<typename StageT>
void ConstantBuffer<DataT>::Bind(ID3D11DeviceContext* device_context, u32 slot) const {
	StageT::BindConstantBuffers(device_context, slot, 1, buffer.GetAddressOf());
}