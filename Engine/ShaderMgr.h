#pragma once

#include "EngineUtil.h"
#include "LightShader.h"
#include <d3d11.h>
#include <variant>
#include <string>
#include <map>
#include <wrl\client.h>

using std::map;
using std::wstring;
using std::shared_ptr;
using std::make_shared;
using Microsoft::WRL::ComPtr;

class ShaderMgr {
	public:
		ShaderMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, HWND hWnd);
		~ShaderMgr();

		void CreateShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const WCHAR* psFilename);
		std::variant<shared_ptr<LightShader>> GetShader(ShaderTypes shaderType, wstring shaderName);


	private:
		void BuildShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const WCHAR* psFilename);


	private:
		map<wstring, shared_ptr<LightShader>> m_LightShaderMap;

		std::variant<LightShader> variant;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		HWND m_hWnd;
};

