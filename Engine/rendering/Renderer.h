#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <wrl\client.h>
#include <memory>

#include "util\EngineUtil.h"
#include "direct3d\Direct3D.h"
#include "scene\Scene.h"
#include "camera\Camera.h"
#include "RenderingMgr.h"
#include "light\Light.h"
#include "rendering\buffer\Buffers.h"

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
		HWND                        hWnd;
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		shared_ptr<Direct3D>        direct3D;
		unique_ptr<RenderingMgr>    renderingMgr;
};

static float rotation = 0.0f;