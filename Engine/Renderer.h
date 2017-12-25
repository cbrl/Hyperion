#pragma once

#include "Direct3D.h"
#include "Scene.h"
#include "Camera.h"
#include "ShaderMgr.h"
#include "Model.h"
#include "Light.h"

#include <d3d11.h>
#include <DirectXColors.h>
#include <wrl\client.h>
#include <variant>

#ifndef GETSHADER_VARIANT
#define GETSHADER_VARIANT variant<monostate, shared_ptr<LightShader>>
#endif

using std::get;
using std::variant;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

class Renderer {
	public:
		Renderer(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~Renderer();

		bool Tick(Direct3D& m_D3DApp, Scene& scene, float deltaTime);

		ComPtr<ID3D11Device> GetDevice();
		ComPtr<ID3D11DeviceContext> GetDeviceContext();


	private:
		HWND                        m_hWnd;
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;

		GETSHADER_VARIANT m_Shader;
};

static float rotation = 0.0f;