#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include <type_traits>
#include <memory>

#include "util\EngineUtil.h"
#include "shader\HlslDefines.h"
#include "rendering\buffer\Buffers.h"
#include "rendering\buffer\ConstantBuffer.h"

using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

class CBufferMgr {
	public:
		CBufferMgr(ID3D11Device* device, ComPtr<ID3D11DeviceContext> deviceContext);
		CBufferMgr();

		void BindCBuffer(BufferTypes buffer);

		template<typename DataT>
		void UpdateData(const DataT& data);

	private:
		void CreateBuffers(ID3D11Device* device);


	private:
		ComPtr<ID3D11DeviceContext> deviceContext;

		unique_ptr<ConstantBuffer<MatrixBuffer>> matrixBuffer;
		unique_ptr<ConstantBuffer<CameraBuffer>> cameraBuffer;
		unique_ptr<ConstantBuffer<LightBuffer>>  lightBuffer;
};


template<typename DataT>
void CBufferMgr::UpdateData(const DataT& data) {
	if constexpr (is_same_v<DataT, MatrixBuffer>) {
		matrixBuffer->UpdateData(deviceContext.Get(), data);
	}

	if constexpr (is_same_v<DataT, CameraBuffer>) {
		cameraBuffer->UpdateData(deviceContext.Get(), data);
	}

	if constexpr (is_same_v<DataT, LightBuffer>) {
		lightBuffer->UpdateData(deviceContext.Get(), data);
	}
}