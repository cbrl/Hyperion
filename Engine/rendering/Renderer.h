#pragma once

#include <wrl\client.h>
#include <d3d11.h>
#include <DirectXColors.h>

#include "util\EngineUtil.h"
#include "direct3d\Direct3D.h"
#include "rendering\buffer\Buffers.h"
#include "scene\Scene.h"
#include "camera\Camera.h"
#include "light\Light.h"
#include "geometry\model\Model2.h"
#include "text\text.h"

using Microsoft::WRL::ComPtr;

class Renderer {
	public:
		static const Renderer* Get();

		Renderer();
		~Renderer();

		void Tick(Scene& scene, float deltaTime) const;


	private:
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;
};

static float rotation = 0.0f;