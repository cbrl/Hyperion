#pragma once

#include <d3d11.h>
#include <string>
#include <map>
#include <wrl\client.h>

using Microsoft::WRL::ComPtr;
using std::wstring;
using std::map;

class ShaderMgr {
	public:
		struct Shader {
			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader>  pixelShader;
		};


	public:
		ShaderMgr();
		~ShaderMgr();

		void CreateShader(wstring shaderName, wstring vertexShaderFilename, wstring pixelShaderFilename);		
		

	private:
		void BuildShader(wstring vertexShaderFilename, wstring pixelShaderFilename);


	private:
		map<wstring, Shader> m_ShaderMap;
};

