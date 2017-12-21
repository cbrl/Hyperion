#pragma once

#include "EngineUtil.h"
#include "LightShader.h"
#include <d3d11.h>
#include <string>
#include <map>
#include <wrl\client.h>

using Microsoft::WRL::ComPtr;
using std::string;
using std::wstring;
using std::map;
using std::unique_ptr;

class ShaderMgr {
	public:
		ShaderMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, HWND hWnd);
		~ShaderMgr();

		void CreateShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const char* vsEntryPoint,
						  const WCHAR* psFilename, const char* psEntryPoint);
		

	private:
		void BuildShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const char* vsEntryPoint,
						 const WCHAR* psFilename, const char* psEntryPoint);


	private:
		map<wstring, LightShader> m_LightShaderMap;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		HWND m_hWnd;
};

