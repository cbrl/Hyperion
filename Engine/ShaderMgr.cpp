#include "stdafx.h"
#include "ShaderMgr.h"

//create a shader type for selecting shader to create/use

ShaderMgr::ShaderMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, HWND hWnd) :
	m_Device(device),
	m_DeviceContext(deviceContext),
	m_hWnd(hWnd)
{
}


ShaderMgr::~ShaderMgr() {
	m_LightShaderMap.clear();
}


void ShaderMgr::CreateShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const char* vsEntryPoint,
							 const WCHAR* psFilename, const char* psEntryPoint) {
	//wstring shaderName = vertexShaderFilename.substr(0, vertexShaderFilename.find('.'));
	switch (shaderType) {
		case ShaderTypes::ColorShader:
			//if (m_ColorShaderMap.find(shaderName) != m_ColorShaderMap.end()) {
			//	return;
			//}
			break;
		
		case ShaderTypes::LightShader:
			if (m_LightShaderMap.find(shaderName) != m_LightShaderMap.end()) {
				return;
			}
			break;

		case ShaderTypes::NormalShader:
			//if (m_NormalShaderMap.find(shaderName) != m_NormalShaderMap.end()) {
			//	return;
			//}
			break;

		case ShaderTypes::SpecularShader:
			//if (m_SpecularShaderMap.find(shaderName) != m_SpecularShaderMap.end()) {
			//	return;
			//}
			break;
	}

	BuildShader(shaderType, shaderName, vsFilename, vsEntryPoint, psFilename, psEntryPoint);
}


void ShaderMgr::BuildShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const char* vsEntryPoint,
							const WCHAR* psFilename, const char* psEntryPoint) {
	switch (shaderType) {
		case ShaderTypes::ColorShader:
			break;

		case ShaderTypes::LightShader:
			m_LightShaderMap[shaderName].Init(m_Device, m_hWnd, vsFilename, vsEntryPoint, psFilename, psEntryPoint);

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}