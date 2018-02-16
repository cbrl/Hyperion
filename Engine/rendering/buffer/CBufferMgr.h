#pragma once

#include <memory>
#include <type_traits>
#include <wrl\client.h>
#include <d3d11.h>

#include "util\EngineUtil.h"
#include "shader\HLSL.h"
#include "direct3d\Direct3D.h"
#include "rendering\buffer\Buffers.h"
#include "rendering\buffer\ConstantBuffer.h"

using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

class CBufferMgr {
	public:
		CBufferMgr();
		~CBufferMgr();

		void BindBuffer(ID3D11DeviceContext* device_context, BufferTypes buffer) const;
		void BindBuffers(ID3D11DeviceContext* device_context) const;

		template<typename DataT>
		void UpdateData(ID3D11DeviceContext* device_context, const DataT& data) const;

	private:
		void CreateBuffers(ID3D11Device* device);


	private:
		unique_ptr<ConstantBuffer<MatrixBuffer>>      matrix_buffer;
		unique_ptr<ConstantBuffer<CameraBuffer>>      camera_buffer;
		unique_ptr<ConstantBuffer<DirectionalLight>>  light_buffer;
};


template<typename DataT>
void CBufferMgr::UpdateData(ID3D11DeviceContext* device_context, const DataT& data) const {
	if constexpr (is_same_v<DataT, MatrixBuffer>) {
		matrix_buffer->UpdateData(device_context, data);
	}

	if constexpr (is_same_v<DataT, CameraBuffer>) {
		camera_buffer->UpdateData(device_context, data);
	}

	if constexpr (is_same_v<DataT, DirectionalLight>) {
		light_buffer->UpdateData(device_context, data);
	}
}