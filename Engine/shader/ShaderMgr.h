#pragma once

#include <map>
#include <stdafx.h>
#include <memory>
#include <Windows.h>
#include <wrl\client.h>
#include <d3d11.h>
#include <VertexTypes.h>

#include "util\EngineUtil.h"
#include "direct3d\direct3d.h"
#include "shader\Shader.h"

using std::map;
using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

struct ShaderMgr {
	ShaderMgr() {
		shaders.try_emplace(ShaderTypes::ColorShader, Shader());
		shaders.try_emplace(ShaderTypes::TextureShader, Shader());
		shaders.try_emplace(ShaderTypes::LightShader, Shader());
		shaders.try_emplace(ShaderTypes::NormalShader, Shader());
		shaders.try_emplace(ShaderTypes::SpecularShader, Shader());
	}
	~ShaderMgr() {
		shaders.clear();
	}

	void Init(HWND hWnd) {
		// Color shader
		shaders.at(ShaderTypes::ColorShader).Init(hWnd, Direct3D::Get()->GetDevice(), L"./shaders/color/color.vs", L"./shaders/color/color.ps",
															 VertexPositionColor::InputElements, VertexPositionColor::InputElementCount);
		
		// Light shader
		shaders.at(ShaderTypes::LightShader).Init(hWnd, Direct3D::Get()->GetDevice(), L"./shaders/light/light.vs", L"./shaders/light/light.ps",
															 VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);

		// Texture shader
		shaders.at(ShaderTypes::TextureShader).Init(hWnd, Direct3D::Get()->GetDevice(), L"./shaders/texture/texture.vs", L"./shaders/texture/texture.ps",
		                                                       VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount);

		// etc...
	}

	void BindShader(ShaderTypes shader) {
		shaders.at(shader).BindShader(Direct3D::Get()->GetDeviceContext());
	}

	private:
		map<ShaderTypes, Shader> shaders;
};