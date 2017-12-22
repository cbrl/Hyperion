#pragma once

#include "EngineUtil.h"
#include <d3d11.h>
#include <DirectXMath.h>

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

		XMFLOAT3 GetDirection();
		XMFLOAT4 GetAmbientColor();
		XMFLOAT4 GetDiffuseColor();
		XMFLOAT4 GetSpecularColor();
		float    GetSpecularPower();


	private:
		XMFLOAT3 m_Direction;
		XMFLOAT4 m_AmbientColor;
		XMFLOAT4 m_DiffuseColor;
		XMFLOAT4 m_SpecularColor;
		float    m_SpecularPower;
};

