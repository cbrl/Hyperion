#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include <memory>

#include "util\EngineUtil.h"
#include "shader\Shader.h"
#include "shader\ShaderMgr.h"
#include "rendering\Sampler.h"

using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

class RenderStateMgr {
	public:
		RenderStateMgr(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~RenderStateMgr();

		bool Init();
		void BindShader(ShaderTypes shader);


	private:
		HWND                        hWnd;
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		unique_ptr<ShaderMgr>       shaderMgr;
		unique_ptr<Sampler>         sampler;
};