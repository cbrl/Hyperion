module;

#include "datatypes/types.h"

#include "directx/directxtk.h"
#include "directx/d3d11.h"

export module rendering.constant_buffer;


export namespace render {

template<typename DataT>
class ConstantBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ConstantBuffer(ID3D11Device& device) {
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
	ConstantBuffer(const ConstantBuffer& buffer) = delete;
	ConstantBuffer(ConstantBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ConstantBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ConstantBuffer& operator=(const ConstantBuffer& buffer) = delete;
	ConstantBuffer& operator=(ConstantBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Map the buffer and copy the new data into it
	void updateData(ID3D11DeviceContext& device_context, const DataT& data) const {
		D3D11_MAPPED_SUBRESOURCE mapped_data = {};

		ThrowIfFailed(device_context.Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data),
					  "Failed to map constant buffer for update");

		memcpy(mapped_data.pData, &data, sizeof(DataT));
		device_context.Unmap(buffer.Get(), 0);
	}

	// Bind the cbuffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) const {
		StageT::bindConstantBuffer(device_context, slot, buffer.Get());
	}


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11Buffer> buffer;
};

} // namespace render
