#pragma once

#include "Direct3D.h"
#include "Camera.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "Light.h"
#include "Model.h"
#include <d3d11.h>
#include <wrl\client.h>
#include <variant>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::get;

class Graphics {
	public:
		Graphics(HWND hWnd, int windowWidth, int windowHeight);
		~Graphics();

		bool Init();
		bool Tick();
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

		std::variant<shared_ptr<LightShader>> m_Shader;

};

static float rotation = 0.0f;