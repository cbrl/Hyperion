#pragma once

#include <d3d11.h>
#include "util/engine_util.h"
#include "datatypes/datatypes.h"
#include "pipeline.h"


template<typename DataT>
struct ConstantBuffer final {
public:
	ConstantBuffer(ID3D11Device& device);

	~ConstantBuffer() = default;

	// Map the buffer and copy the new data into it
	void updateData(ID3D11DeviceContext& device_context, const DataT& data) const;

	// Bind the cbuffer to the specified pipeline stage
	template<typename StageT>
	void bind(ID3D11DeviceContext& device_context, u32 slot) const {
		StageT::bindConstantBuffers(device_context, slot, 1, buffer.GetAddressOf());
	}


private:
	ComPtr<ID3D11Buffer> buffer;
};


#include "constant_buffer.tpp"
