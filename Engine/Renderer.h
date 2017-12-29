#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <wrl\client.h>
#include <variant>

#include "Direct3D.h"
#include "Scene.h"
#include "Camera.h"
#include "ShaderMgr.h"
#include "Model.h"
#include "Light.h"
#include "BufferTypes.h"

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

class Renderer {
	public:
		Renderer(HWND hWnd, shared_ptr<Direct3D> direct3D);
		~Renderer();

		bool Init();
		bool Tick(Scene& scene, float deltaTime);


	private:
		HWND                        m_hWnd;
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		shared_ptr<Direct3D>        m_Direct3D;
		unique_ptr<ShaderMgr>       m_ShaderMgr;
};

static float rotation = 0.0f;