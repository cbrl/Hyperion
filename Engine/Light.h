#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "EngineUtil.h"
#include "BufferTypes.h"

using namespace DirectX;

class Light {
	public:
		Light();
		~Light();

		void SetDirection(XMFLOAT3 direction);
		void SetAmbientColor(XMFLOAT4 color);
		void SetDiffuseColor(XMFLOAT4 color);
		void SetSpecularColor(XMFLOAT4 color);
		void SetSpecularPower(float power);

		LightBuffer GetBuffer();
		XMFLOAT3    GetDirection();
		XMFLOAT4    GetAmbientColor();
		XMFLOAT4    GetDiffuseColor();
		XMFLOAT4    GetSpecularColor();
		float       GetSpecularPower();


	private:
		LightBuffer m_Buffer;
};
