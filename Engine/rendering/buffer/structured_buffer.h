#pragma once

#include <d3d11.h>
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


template<typename DataT>
class StructuredBuffer final {
	public:
		StructuredBuffer(ID3D11Device& device, u32 size) : size(size) {
			CreateBuffer(device);
		}

		~StructuredBuffer() = default;


		void UpdateData(ID3D11Device& device,
						ID3D11DeviceContext& device_context,
						const vector<DataT>& data);


		// Bind the buffer to the specified pipeline stage
		template<typename StageT>
		void Bind(ID3D11DeviceContext& device_context, u32 slot) {
			StageT::BindSRVs(device_context, slot, 1, srv.GetAddressOf());
		}


	private:
		void CreateBuffer(ID3D11Device& device);


	private:
		ComPtr<ID3D11Buffer> buffer;
		ComPtr<ID3D11ShaderResourceView> srv;

		// Number of elements in the buffer
		u32 size;
};


#include "structured_buffer.tpp"