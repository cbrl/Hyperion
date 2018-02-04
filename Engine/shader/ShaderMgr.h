#pragma once

#include <map>

#include <stdafx.h>
#include <d3d11.h>
#include <memory>
#include <Windows.h>
#include <wrl\client.h>
#include <VertexTypes.h>
#include "util\EngineUtil.h"
#include "shader\Shader.h"

using std::map;
using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

struct ShaderMgr {
	ShaderMgr() {
		m_Shaders.try_emplace(ShaderTypes::ColorShader, Shader());
		m_Shaders.try_emplace(ShaderTypes::TextureShader, Shader());
		m_Shaders.try_emplace(ShaderTypes::LightShader, Shader());
		m_Shaders.try_emplace(ShaderTypes::NormalShader, Shader());
		m_Shaders.try_emplace(ShaderTypes::SpecularShader, Shader());
	}
	~ShaderMgr() {
		m_Shaders.clear();
	}

	void Init(HWND hWnd, ID3D11Device* device) {
		// Color shader
		m_Shaders.at(ShaderTypes::ColorShader).Init(hWnd, device, L"./shaders/color/color.vs", L"./shaders/color/color.ps", 
															 VertexPositionColor::InputElements, VertexPositionColor::InputElementCount);
		
		// Light shader
		m_Shaders.at(ShaderTypes::LightShader).Init(hWnd, device, L"./shaders/light/light.vs", L"./shaders/light/light.ps",
															 VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);

		// Texture shader
		m_Shaders.at(ShaderTypes::TextureShader).Init(hWnd, device, L"./shaders/texture/texture.vs", L"./shaders/texture/texture.ps",
		                                                       VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount);

		// etc...
	}

	void BindShader(ID3D11DeviceContext* deviceContext, ShaderTypes shader) {
		m_Shaders.at(shader).BindShader(deviceContext);
	}

	private:
		map<ShaderTypes, Shader> m_Shaders;
};