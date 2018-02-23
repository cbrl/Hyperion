#pragma once

#include "util\math\math.h"

using namespace DirectX;


//----------------------------------------------------------------------------------
// Matrix Buffer
//----------------------------------------------------------------------------------

struct MatrixBuffer {
	MatrixBuffer() : world(XMMatrixIdentity()), view(XMMatrixIdentity()), projection(XMMatrixIdentity()) {}
	MatrixBuffer(XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix)
		: world(world_matrix)
		, view(view_matrix)
		, projection(projection_matrix)
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
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	MaterialBuffer()
		: ambient(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, specular_power(1.0f)
		, optical_density(0.0f)
		, dissolve(0.0f)
		, pad(0.0f)
	{}
	MaterialBuffer(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, XMFLOAT4 emissive,
				   float specular_power, float optical_density, float dissolve)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, emissive(emissive)
		, specular_power(specular_power)
		, dissolve(dissolve)
		, pad(0.0f)
	{}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 emissive;
	float    specular_power;
	float    optical_density;
	float    dissolve;
	float    pad;
};

struct ModelBuffer {
	ModelBuffer()
		: world(XMMatrixIdentity())
		, world_inv_transpose(XMMatrixIdentity())
		, world_view_proj(XMMatrixIdentity())
		, texTransform(XMMatrixIdentity())
	{}
	XMMATRIX world;
	XMMATRIX world_inv_transpose;
	XMMATRIX world_view_proj;
	XMMATRIX texTransform;
	MaterialBuffer mat;
};



//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------

struct LightBuffer {
	LightBuffer()
		: point_light_count(0)
		, directional_light_count(0)
		, spot_light_count(0)
		, pad(0)
		, fog_color(1.0f, 1.0f, 1.0f, 1.0f)
		, fog_start(0.0f)
		, fog_range(0.0f)
		, pad2(0.0f, 0.0f)
	{}
	LightBuffer(unsigned int point_lights, unsigned int directional_lights, unsigned int spot_lights,
				XMFLOAT4 fog_color, float fog_start, float fog_range)
		: directional_light_count(directional_lights)
		, point_light_count(point_lights)
		, spot_light_count(spot_lights)
		, pad(0)
		, fog_color(fog_color)
		, fog_start(fog_start)
		, fog_range(fog_range)
		, pad2(0.0f, 0.0f)
	{}
	unsigned int directional_light_count;
	unsigned int point_light_count;
	unsigned int spot_light_count;
	unsigned int pad;

	XMFLOAT4 fog_color;
	float    fog_start;
	float    fog_range;
	XMFLOAT2 pad2;
};


struct DirectionalLight {
	DirectionalLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, pad(0.0f)
	{}
	DirectionalLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 direction, XMFLOAT4 specular)
		: ambient_color(ambient)
		, diffuse_color(diffuse)
		, direction(direction)
		, specular(specular)
		, pad(0.0f)
	{}

	XMFLOAT4 ambient_color;
	XMFLOAT4 diffuse_color;
	XMFLOAT4 specular; //specular.w is the specular power
	XMFLOAT3 direction;
	float    pad;
};


struct PointLight {
	PointLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 1.0f)
		, range(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
		, pad(0.0f)
	{}
	PointLight(XMFLOAT4 ambient_color, XMFLOAT4 diffuse_color, XMFLOAT4 specular,
			   XMFLOAT3 position, float range, XMFLOAT3 attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, attenuation(attenuation)
		, pad(0.0f)
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
	SpotLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 0.0f)
		, range(0.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, spot(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
		, pad(0.0f)
	{}
	SpotLight(XMFLOAT4 ambient_color, XMFLOAT4 diffuse_color, XMFLOAT4 specular, XMFLOAT3 position,
	          float range, XMFLOAT3 direction, float spot, XMFLOAT3 attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, direction(direction)
		, spot(spot)
		, attenuation(attenuation)
		, pad(0.0f)
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