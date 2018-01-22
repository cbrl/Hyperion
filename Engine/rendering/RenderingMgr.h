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
			cBufferMgr->UpdateData(data);
		}


	private:
		HWND                        hWnd;
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		unique_ptr<ShaderMgr>       shaderMgr;
		unique_ptr<CBufferMgr>      cBufferMgr;
		unique_ptr<Sampler>         sampler;
};