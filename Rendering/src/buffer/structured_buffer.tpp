template<typename DataT>
void StructuredBuffer<DataT>::updateData(ID3D11Device& device,
                                         ID3D11DeviceContext& device_context,
                                         const std::vector<DataT>& data) {
	// Set the number of elements used
	current_size = static_cast<u32>(data.size());

	if (data.size() == 0) return;

	// Recreate the buffer if the array of data being fed to it is larger than the buffer
	if (current_size > reserved_size) {
		reserved_size = current_size;
		createBuffer(device);
	}

	// Map the buffer and update the data
	D3D11_MAPPED_SUBRESOURCE mapped_data = {};

	ThrowIfFailed(device_context.Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_data),
	              "Failed to std::map structured buffer");

	memcpy(mapped_data.pData, data.data(), sizeof(DataT) * current_size);

	device_context.Unmap(buffer.Get(), NULL);
}


template<typename DataT>
void StructuredBuffer<DataT>::createBuffer(ID3D11Device& device) {

	// Create the buffer
	D3D11_BUFFER_DESC desc = {};

	desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.Usage               = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth           = sizeof(DataT) * reserved_size;
	desc.StructureByteStride = sizeof(DataT);

	ThrowIfFailed(device.CreateBuffer(&desc, nullptr, buffer.ReleaseAndGetAddressOf()),
	              "Failed to create structured buffer");

	SetDebugObjectName(buffer.Get(), "Structured Buffer");


	// Create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Buffer.FirstElement             = 0;
	srv_desc.Buffer.NumElements              = reserved_size;
	srv_desc.Format                          = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension                   = D3D11_SRV_DIMENSION_BUFFER;

	ThrowIfFailed(device.CreateShaderResourceView(buffer.Get(), &srv_desc, srv.ReleaseAndGetAddressOf()),
	              "Failed to create structured buffer SRV");

	SetDebugObjectName(srv.Get(), "Structured Buffer SRV");
}
