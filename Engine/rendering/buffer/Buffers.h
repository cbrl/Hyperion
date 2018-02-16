#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


//----------------------------------------------------------------------------------
// Buffer Type enum
//----------------------------------------------------------------------------------

enum class BufferTypes {
	MatrixBuffer,
	CameraBuffer,
	DirectionalLight,
	PointLight,
	SpotLight
};


//----------------------------------------------------------------------------------
// Matrix Buffer
//----------------------------------------------------------------------------------
struct MatrixBuffer {
	MatrixBuffer() : world(XMMatrixIdentity()), view(XMMatrixIdentity()), projection(XMMatrixIdentity()) {}
	MatrixBuffer(XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix) :
		world(world_matrix),
		view(view_matrix),
		projection(projection_matrix)
	{}
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};


//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------
struct CameraBuffer {
	CameraBuffer() : position(0.0f, 0.0f, 0.0f), padding(0.0f) {}
	CameraBuffer(XMFLOAT3 pos) : position(pos), padding(0.0f) {}
	XMFLOAT3 position;
	float    padding;
};


//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------
struct DirectionalLight {
	DirectionalLight() :
		ambient_color(0.0f, 0.0f, 0.0f, 0.0f),
		diffuse_color(0.0f, 0.0f, 0.0f, 0.0f),
		direction(0.0f, 0.0f, 0.0f),
		specular(0.0f, 0.0f, 0.0f, 0.0f),
		pad(0.0f)
	{}
	DirectionalLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 direction, XMFLOAT4 specular) :
		ambient_color(ambient),
		diffuse_color(diffuse),
		direction(direction),
		specular(specular),
		pad(0.0f)
	{}

	XMFLOAT4 ambient_color;
	XMFLOAT4 diffuse_color;
	XMFLOAT3 direction;
	float    pad;
	XMFLOAT4 specular; //specular.w is the specular power
};


struct PointLight {
	PointLight() :
		ambient_color(0.0f, 0.0f, 0.0f, 0.0f),
		diffuse_color(0.0f, 0.0f, 0.0f, 0.0f),
		specular(0.0f, 0.0f, 0.0f, 0.0f),
		position(0.0f, 0.0f, 0.0f),
		range(0.0f),
		attenuation(0.0f, 0.0f, 0.0f),
		pad(0.0f)
	{}
	PointLight(XMFLOAT4 ambient_color, XMFLOAT4 diffuse_color, XMFLOAT4 specular,
	           XMFLOAT3 position, float range, XMFLOAT3 attenuation) :
		ambient_color(ambient_color),
		diffuse_color(diffuse_color),
		specular(specular),
		position(position),
		range(range),
		attenuation(attenuation),
		pad(0.0f)
	{}

	XMFLOAT4 ambient_color;
	XMFLOAT4 diffuse_color;
	XMFLOAT4 specular; //specular.w is the specular power
	XMFLOAT3 position;
	float    range;
	XMFLOAT3 attenuation;
	float    pad;
};


struct SpotLight {
	SpotLight() :
		ambient_color(0.0f, 0.0f, 0.0f, 0.0f),
		diffuse_color(0.0f, 0.0f, 0.0f, 0.0f),
		specular(0.0f, 0.0f, 0.0f, 0.0f),
		position(0.0f, 0.0f, 0.0f),
		range(0.0f),
		direction(0.0f, 0.0f, 0.0f),
		spot(0.0f),
		attenuation(0.0f, 0.0f, 0.0f),
		pad(0.0f)
	{}
	SpotLight(XMFLOAT4 ambient_color, XMFLOAT4 diffuse_color, XMFLOAT4 specular, XMFLOAT3 position,
	          float range, XMFLOAT3 direction, float spot, XMFLOAT3 attenuation) :
		ambient_color(ambient_color),
		diffuse_color(diffuse_color),
		specular(specular),
		position(position),
		range(range),
		direction(direction),
		spot(spot),
		attenuation(attenuation),
		pad(0.0f)
	{}

	XMFLOAT4 ambient_color;
	XMFLOAT4 diffuse_color;
	XMFLOAT4 specular; //specular.w is the specular power
	XMFLOAT3 position;
	float    range;
	XMFLOAT3 direction;
	float    spot;
	XMFLOAT3 attenuation;
	float    pad;
};