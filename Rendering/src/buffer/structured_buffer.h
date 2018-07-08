#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"


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
	                const vector<DataT>& data);


	// Bind the buffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) {
		StageT::bindSRVs(device_context, slot, 1, srv.GetAddressOf());
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


private:
	ComPtr<ID3D11Buffer> buffer;
	ComPtr<ID3D11ShaderResourceView> srv;

	// Number of elements in the buffer
	u32 current_size;

	// Max elements buffer can hold
	u32 reserved_size;
};


#include "structured_buffer.tpp"
