#pragma once

#include <d3d11.h>
#include <VertexTypes.h>
#include <wrl\client.h>
#include <type_traits>

#include "EngineUtil.h"
#include "Shader.h"
#include "CBuffers.h"
#include "BufferTypes.h"

using std::unique_ptr;
using std::make_unique;
using std::is_same_v;
using Microsoft::WRL::ComPtr;

class ShaderMgr {
	public:
		ShaderMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~ShaderMgr();

		bool Init();
		void BindShader(ShaderTypes shader);

		template<typename DataT>
		void UpdataData(const DataT& data);

	public:
		unique_ptr<Shader> m_LightShader;


	private:
		HWND                          m_hWnd;
		ComPtr<ID3D11Device>          m_Device;
		ComPtr<ID3D11DeviceContext>   m_DeviceContext;
		unique_ptr<CBuffers>          m_CBuffers;
};


template<typename DataT>
void ShaderMgr::UpdataData(const DataT& data) {
	if constexpr (is_same_v<DataT, MatrixBuffer>) {
		m_CBuffers->matrixBuffer->UpdateData(m_DeviceContext, data);
	}

	if constexpr (is_same_v<DataT, CameraBuffer>) {
		m_CBuffers->cameraBuffer->UpdateData(m_DeviceContext, data);
	}
		
	if constexpr (is_same_v<DataT, LightBuffer>) {
		m_CBuffers->lightBuffer->UpdateData(m_DeviceContext, data);
	}
}