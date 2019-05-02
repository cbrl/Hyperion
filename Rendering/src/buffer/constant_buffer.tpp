
namespace render {

template<typename DataT>
ConstantBuffer<DataT>::ConstantBuffer(ID3D11Device& device) {
	D3D11_BUFFER_DESC buffer_desc = {};

	buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth           = sizeof(DataT);
	buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags           = NULL;
	buffer_desc.StructureByteStride = NULL;

	ThrowIfFailed(device.CreateBuffer(&buffer_desc, nullptr, buffer.ReleaseAndGetAddressOf()),
	              "Failed to create constant buffer");

	SetDebugObjectName(buffer.Get(), "Constant Buffer");
}


template<typename DataT>
void ConstantBuffer<DataT>::updateData(ID3D11DeviceContext& device_context, const DataT& data) const {
	D3D11_MAPPED_SUBRESOURCE mapped_data = {};

	ThrowIfFailed(device_context.Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
	              "Failed to map constant buffer for update");

	memcpy(mapped_data.pData, &data, sizeof(DataT));
	//*(DataT*)mapped_data.pData = data;

	device_context.Unmap(buffer.Get(), NULL);
}

} // namespace render
