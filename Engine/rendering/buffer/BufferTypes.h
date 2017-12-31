#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


// Vertex Shader Slots
#define MATRIX_BUFFER_SLOT 0
#define CAMERA_BUFFER_SLOT 1

// Pixel Shader Slots
#define LIGHT_BUFFER_SLOT  0


struct MatrixBuffer {
	MatrixBuffer() = default;
	MatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) {
		world      = worldMatrix;
		view       = viewMatrix;
		projection = projectionMatrix;
	}
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};


struct CameraBuffer {
	CameraBuffer() = default;
	CameraBuffer(XMFLOAT3 position) {
		cameraPosition = position;
	}
	XMFLOAT3 cameraPosition;
	float    padding;
};


struct LightBuffer {
	LightBuffer() = default;
	LightBuffer(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 direction, float specPower, XMFLOAT4 specColor) {
		ambientColor   = ambient;
		diffuseColor   = diffuse;
		lightDirection = direction;
		specularPower  = specPower;
		specularColor  = specColor;
	}
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float    specularPower;
	XMFLOAT4 specularColor;
};