#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct MatrixBuffer {
	MatrixBuffer() : world({}), view({}), projection({}) {}
	MatrixBuffer(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) :
		world(worldMatrix),
		view(viewMatrix),
		projection(projectionMatrix)
	{}
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};


struct CameraBuffer {
	CameraBuffer() : position(0.0f, 0.0f, 0.0f), padding(0.0f) {}
	CameraBuffer(XMFLOAT3 pos) : position(pos) {}
	XMFLOAT3 position;
	float    padding;
};


struct LightBuffer {
	LightBuffer() :
		ambientColor(0.0f, 0.0f, 0.0f, 0.0f),
		diffuseColor(0.0f, 0.0f, 0.0f, 0.0f),
		lightDirection(0.0f, 0.0f, 0.0f),
		specularPower(0.0f),
		specularColor(0.0f, 0.0f, 0.0f, 0.0f)
	{}
	LightBuffer(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 direction, float specPower, XMFLOAT4 specColor) :
		ambientColor(ambient),
		diffuseColor(diffuse),
		lightDirection(direction),
		specularPower(specPower),
		specularColor(specColor)
	{}
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float    specularPower;
	XMFLOAT4 specularColor;
};