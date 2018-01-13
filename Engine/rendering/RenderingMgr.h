#pragma once

#include <d3d11.h>
#include <VertexTypes.h>
#include <wrl\client.h>
#include <memory>

#include "util\EngineUtil.h"
#include "shader\Shader.h"
#include "shader\ShaderMgr.h"
#include "rendering\buffer\CBufferMgr.h"
#include "rendering\buffer\Buffers.h"
#include "rendering\Sampler.h"

using std::unique_ptr;
using std::make_unique;
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


	private:
		HWND                        m_hWnd;
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		unique_ptr<ShaderMgr>       m_ShaderMgr;
		unique_ptr<CBufferMgr>      m_CBufferMgr;
		unique_ptr<Sampler>         m_Sampler;
};