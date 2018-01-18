#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "rendering\buffer\Buffers.h"

using namespace DirectX;

struct Light {
	public:
		Light()  = default;
		~Light() = default;

		void SetDirection(XMFLOAT3 direction) { m_Buffer.lightDirection = direction; }
		void SetAmbientColor(XMFLOAT4 color)  { m_Buffer.ambientColor = color; }
		void SetDiffuseColor(XMFLOAT4 color)  { m_Buffer.diffuseColor = color; }
		void SetSpecularColor(XMFLOAT4 color) { m_Buffer.specularColor = color; }
		void SetSpecularPower(float power)    { m_Buffer.specularPower = power; }

		LightBuffer GetBuffer()        { return m_Buffer; }
		XMFLOAT3    GetDirection()     { return m_Buffer.lightDirection; }
		XMFLOAT4    GetAmbientColor()  { return m_Buffer.ambientColor; }
		XMFLOAT4    GetDiffuseColor()  { return m_Buffer.diffuseColor; }
		XMFLOAT4    GetSpecularColor() { return m_Buffer.specularColor; }
		float       GetSpecularPower() { return m_Buffer.specularPower; }


	private:
		LightBuffer m_Buffer;
};

