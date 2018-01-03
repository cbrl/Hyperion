#pragma once

#include <d3d11.h>
#include <VertexTypes.h>
#include <wrl\client.h>
#include <type_traits>

#include "util\EngineUtil.h"
#include "shader\Shader.h"
#include "rendering\buffer\CBufferMgr.h"
#include "rendering\buffer\Buffers.h"

using std::unique_ptr;
using std::make_unique;
using std::is_same_v;
using Microsoft::WRL::ComPtr;

class RenderingMgr {
	public:
		RenderingMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~RenderingMgr();

		bool Init();
		void BindShader(ShaderTypes shader);

		template<typename DataT>
		void UpdateData(const DataT& data) {
			m_CBufferMgr->UpdateData(data);
		}


	public:
		unique_ptr<Shader> m_LightShader;


	private:
		HWND                          m_hWnd;
		ComPtr<ID3D11Device>          m_Device;
		ComPtr<ID3D11DeviceContext>   m_DeviceContext;
		unique_ptr<CBufferMgr>        m_CBufferMgr;
};