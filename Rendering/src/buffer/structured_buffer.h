#pragma once

#include "directx/d3d11.h"
#include "datatypes/types.h"


namespace render {

template<typename DataT>
class StructuredBuffer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	StructuredBuffer(ID3D11Device& device, u32 reserved_size)
		: current_size(0)
		, reserved_size(reserved_size) {

		createBuffer(device);
	}

	StructuredBuffer(const StructuredBuffer& buffer) = delete;
	StructuredBuffer(StructuredBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~StructuredBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	StructuredBuffer& operator=(const StructuredBuffer& buffer) = delete;
	StructuredBuffer& operator=(StructuredBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void updateData(ID3D11Device& device,
	                ID3D11DeviceContext& device_context,
	                const std::vector<DataT>& data);


	// Bind the buffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) {
		StageT::bindSRV(device_context, slot, srv.Get());
	}


	// Get the number of elements this buffer currently holds
	[[nodiscard]]
	u32 size() const {
		return current_size;
	}

	// Get the max number of elements this buffer can currently hold
	[[nodiscard]]
	u32 reserved() const {
		return reserved_size;
	}

private:

	void createBuffer(ID3D11Device& device);


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11Buffer> buffer;
	ComPtr<ID3D11ShaderResourceView> srv;

	// Number of elements in the buffer
	u32 current_size;

	// Max elements buffer can hold
	u32 reserved_size;
};

} //namespace render

#include "structured_buffer.tpp"