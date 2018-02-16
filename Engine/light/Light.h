#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "rendering\buffer\Buffers.h"

using namespace DirectX;

struct Light {
	public:
		Light()  = default;
		~Light() = default;

		void SetDirection(XMFLOAT3 direction) { buffer.direction = direction; }
		void SetAmbientColor(XMFLOAT4 color)  { buffer.ambient_color = color; }
		void SetDiffuseColor(XMFLOAT4 color)  { buffer.diffuse_color = color; }
		void SetSpecularPower(float power)    { buffer.specular.w = power; }
		void SetSpecularColor(XMFLOAT3 color) {
			buffer.specular.x = color.x;
			buffer.specular.y = color.y;
			buffer.specular.z = color.z;
		}

		DirectionalLight GetBuffer()        { return buffer; }
		XMFLOAT3    GetDirection()     { return buffer.direction; }
		XMFLOAT4    GetAmbientColor()  { return buffer.ambient_color; }
		XMFLOAT4    GetDiffuseColor()  { return buffer.diffuse_color; }
		XMFLOAT3    GetSpecularColor() { return XMFLOAT3(buffer.specular.x, buffer.specular.y, buffer.specular.z); }
		float       GetSpecularPower() { return buffer.specular.w; }


	private:
		DirectionalLight buffer;
};

