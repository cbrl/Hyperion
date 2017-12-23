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


bool ShaderMgr::CreateShader(ShaderTypes shaderType, const WCHAR* vsFilename, const WCHAR* psFilename) {
	bool result;

	if (m_LightShaderMap.find(shaderType) != m_LightShaderMap.end()) {
		result = true;
	}
	else {
		result = BuildShader(shaderType, vsFilename, psFilename);
	}

	return result;
}


bool ShaderMgr::BuildShader(ShaderTypes shaderType, const WCHAR* vsFilename, const WCHAR* psFilename) {
	bool result;

	switch (shaderType) {
		case ShaderTypes::TextureShader:
			break;

		case ShaderTypes::LightShader:
			m_LightShaderMap[shaderType] = make_shared<LightShader>();
			result = m_LightShaderMap[shaderType]->Init(m_Device, m_hWnd, vsFilename, psFilename);

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}

	return result;
}


GETSHADER_VARIANT ShaderMgr::GetShader(ShaderTypes shaderType) {
	GETSHADER_VARIANT shader;

	if (m_LightShaderMap.find(shaderType) == m_LightShaderMap.end()) {
		return shader;
	}

	switch (shaderType) {
		case ShaderTypes::TextureShader:
			break;

		case ShaderTypes::LightShader:
			shader = m_LightShaderMap[shaderType];
			return shader;

		case ShaderTypes::NormalShader:
			break;

		case ShaderTypes::SpecularShader:
			break;
	}
}