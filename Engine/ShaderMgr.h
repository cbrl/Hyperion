#pragma once

#include <d3d11.h>
#include <variant>
#include <string>
#include <map>
#include <wrl\client.h>
#include "EngineUtil.h"
#include "LightShader.h"

using std::map;
using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::variant;
using std::monostate;
using Microsoft::WRL::ComPtr;

class ShaderMgr {
	public:
		ShaderMgr(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, HWND hWnd);
		~ShaderMgr();

		bool Init();


	public:
		unique_ptr<LightShader> m_LightShader;

	private:
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		HWND m_hWnd;
};

