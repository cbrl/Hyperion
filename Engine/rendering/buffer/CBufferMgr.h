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
		CBufferMgr(const ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext> deviceContext);
		CBufferMgr();

		void BindCBuffer(BufferTypes m_Buffer);

		template<typename DataT>
		void UpdateData(const DataT& data);

	private:
		void CreateBuffers(const ComPtr<ID3D11Device>& device);


	private:
		ComPtr<ID3D11DeviceContext> m_DeviceContext;

		unique_ptr<ConstantBuffer<MatrixBuffer>> m_MatrixBuffer;
		unique_ptr<ConstantBuffer<CameraBuffer>> m_CameraBuffer;
		unique_ptr<ConstantBuffer<LightBuffer>>  m_LightBuffer;
};


template<typename DataT>
void CBufferMgr::UpdateData(const DataT& data) {
	if constexpr (is_same_v<DataT, MatrixBuffer>) {
		m_MatrixBuffer->UpdateData(m_DeviceContext, data);
	}

	if constexpr (is_same_v<DataT, CameraBuffer>) {
		m_CameraBuffer->UpdateData(m_DeviceContext, data);
	}

	if constexpr (is_same_v<DataT, LightBuffer>) {
		m_LightBuffer->UpdateData(m_DeviceContext, data);
	}
}