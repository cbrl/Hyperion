#pragma once

#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "datatypes/datatypes.h"


namespace render {

template<typename DataT>
class ConstantBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ConstantBuffer(ID3D11Device& device);
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
	void updateData(ID3D11DeviceContext& device_context, const DataT& data) const;

	// Bind the cbuffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) const {
		StageT::bindConstantBuffer(device_context, slot, buffer.Get());
	}


private:
	ComPtr<ID3D11Buffer> buffer;
};

} // namespace render

#include "constant_buffer.tpp"