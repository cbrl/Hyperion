#pragma once

#include "EngineUtil.h"
#include "LightShader.h"
#include <d3d11.h>
#include <variant>
#include <string>
#include <map>
#include <wrl\client.h>

#ifndef GETSHADER_VARIANT
#define GETSHADER_VARIANT variant<monostate, shared_ptr<LightShader>>
#endif

using std::map;
using std::wstring;
using std::shared_ptr;
using std::make_shared;
using std::variant;
using std::monostate;
using Microsoft::WRL::ComPtr;

class ShaderMgr {
	public:
		ShaderMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, HWND hWnd);
		~ShaderMgr();

		bool CreateShader(ShaderTypes shaderType, const WCHAR* vsFilename, const WCHAR* psFilename);
		variant<monostate, shared_ptr<LightShader>> GetShader(ShaderTypes shaderType);


	private:
		bool BuildShader(ShaderTypes shaderType, const WCHAR* vsFilename, const WCHAR* psFilename);


	private:
		map<ShaderTypes, shared_ptr<LightShader>> m_LightShaderMap;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		HWND m_hWnd;
};

