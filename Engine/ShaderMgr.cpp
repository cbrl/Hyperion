#include "stdafx.h"
#include "ShaderMgr.h"


ShaderMgr::ShaderMgr() {
}


ShaderMgr::~ShaderMgr() {
}


void ShaderMgr::CreateShader(wstring shaderName, wstring vertexShaderFilename, wstring pixelShaderFilename) {
	if (m_ShaderMap.find(shaderName) != m_ShaderMap.end()) {
		return;
	}
	else {
		BuildShader(vertexShaderFilename, pixelShaderFilename);
	}
}


void BuildShader(wstring vertexShaderFilename, wstring pixelShaderFilename) {

}