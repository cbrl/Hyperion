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


void ShaderMgr::CreateShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const WCHAR* psFilename) {
	//wstring shaderName = vertexShaderFilename.substr(0, vertexShaderFilename.find('.'));
	switch (shaderType) {
		case ShaderTypes::ColorShader:
			//if (m_ColorShaderMap.find(shaderName) != m_ColorShaderMap.end()) {
			//	return;
			//}
			break;

		case ShaderTypes::TextureShader:
			//if (m_TextureShaderMap.find(shaderName) != m_TextureShaderMap.end()) {
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

	BuildShader(shaderType, shaderName, vsFilename, psFilename);
}


void ShaderMgr::BuildShader(ShaderTypes shaderType, wstring shaderName, const WCHAR* vsFilename, const WCHAR* psFilename) {
	switch (shaderType) {
		case ShaderTypes::ColorShader:
			break;

		case ShaderTypes::TextureShader:
			break;

		case ShaderTypes::LightShader:
			m_LightShaderMap[shaderName] = make_shared<LightShader>();
			m_LightShaderMap[shaderName]->Init(m_Device, m_hWnd, vsFilename, psFilename);

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}


std::variant<shared_ptr<LightShader>> ShaderMgr::GetShader(ShaderTypes shaderType, wstring shaderName) {
	std::variant<shared_ptr<LightShader>> shader;

	switch (shaderType) {
		case ShaderTypes::ColorShader:
			break;

		case ShaderTypes::TextureShader:
			break;

		case ShaderTypes::LightShader:
			shader = m_LightShaderMap[shaderName];
			return shader;

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}