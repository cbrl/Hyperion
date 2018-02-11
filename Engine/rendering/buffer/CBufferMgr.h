#pragma once

#include <memory>
#include <type_traits>
#include <wrl\client.h>
#include <d3d11.h>

#include "util\EngineUtil.h"
#include "shader\HlslDefines.h"
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

		void BindBuffer(ID3D11DeviceContext* deviceContext, BufferTypes buffer);
		void BindBuffers(ID3D11DeviceContext* deviceContext);

		template<typename DataT>
		void UpdateData(ID3D11DeviceContext* deviceContext, const DataT& data);

	private:
		void CreateBuffers(ID3D11Device* device);


	private:
		unique_ptr<ConstantBuffer<MatrixBuffer>> matrixBuffer;
		unique_ptr<ConstantBuffer<CameraBuffer>> cameraBuffer;
		unique_ptr<ConstantBuffer<LightBuffer>>  lightBuffer;
};


template<typename DataT>
void CBufferMgr::UpdateData(ID3D11DeviceContext* deviceContext, const DataT& data) {
	if constexpr (is_same_v<DataT, MatrixBuffer>) {
		matrixBuffer->UpdateData(deviceContext, data);
	}

	if constexpr (is_same_v<DataT, CameraBuffer>) {
		cameraBuffer->UpdateData(deviceContext, data);
	}

	if constexpr (is_same_v<DataT, LightBuffer>) {
		lightBuffer->UpdateData(deviceContext, data);
	}
}