#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "rendering\buffer\Buffers.h"

using namespace DirectX;

struct Light {
	public:
		Light()  = default;
		~Light() = default;

		void SetDirection(XMFLOAT3 direction) { buffer.lightDirection = direction; }
		void SetAmbientColor(XMFLOAT4 color)  { buffer.ambientColor = color; }
		void SetDiffuseColor(XMFLOAT4 color)  { buffer.diffuseColor = color; }
		void SetSpecularColor(XMFLOAT4 color) { buffer.specularColor = color; }
		void SetSpecularPower(float power)    { buffer.specularPower = power; }

		LightBuffer GetBuffer()        { return buffer; }
		XMFLOAT3    GetDirection()     { return buffer.lightDirection; }
		XMFLOAT4    GetAmbientColor()  { return buffer.ambientColor; }
		XMFLOAT4    GetDiffuseColor()  { return buffer.diffuseColor; }
		XMFLOAT4    GetSpecularColor() { return buffer.specularColor; }
		float       GetSpecularPower() { return buffer.specularPower; }


	private:
		LightBuffer buffer;
};

