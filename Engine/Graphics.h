#pragma once

#include "Direct3D.h"
#include "Camera.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "Light.h"
#include "Model.h"
#include <d3d11.h>
#include <DirectXColors.h>
#include <wrl\client.h>
#include <variant>

#define MSAA_STATE       false
#define VSYNC_STATE      true
#define FULLSCREEN_STATE false

#ifndef GETSHADER_VARIANT
#define GETSHADER_VARIANT variant<monostate, shared_ptr<LightShader>>
#endif

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::get;
using std::variant;
using Microsoft::WRL::ComPtr;

class Graphics {
	public:
		Graphics(HWND hWnd, int windowWidth, int windowHeight);
		~Graphics();

		bool Init();
		bool Tick(float deltaTime);
		void OnResize(int windowWidth, int windowHeight);


	private:
		unique_ptr<Direct3D>   m_D3DApp;
		unique_ptr<Camera>     m_Camera;
		unique_ptr<ShaderMgr>  m_ShaderMgr;
		unique_ptr<TextureMgr> m_TextureMgr;
		unique_ptr<Light>      m_Light;
		unique_ptr<Model>      m_Model;

		HWND      m_hWnd;
		int       m_WindowWidth;
		int       m_WindowHeight;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;

		GETSHADER_VARIANT m_Shader;
};

static float rotation = 0.0f;