#pragma once

#include "util\datatypes\datatypes.h"
#include "util\math\math.h"


//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct CameraBuffer {
	CameraBuffer() : position(0.0f, 0.0f, 0.0f), padding(0.0f) {}
	CameraBuffer(float3 pos) : position(pos), padding(0.0f) {}
	CameraBuffer(XMVECTOR pos) { XMStoreFloat3(&position, pos); }
	float3 position;
	float  padding;
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
		, optical_density(0.0f)
		, dissolve(0.0f)
		, pad(0.0f, 0.0f)
	{}
	MaterialBuffer(float4 ambient, float4 diffuse, float4 specular, float4 emissive,
				   float specular_power, float optical_density, float dissolve)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, emissive(emissive)
		, dissolve(dissolve)
		, pad(0.0f, 0.0f)
	{}
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
	float  optical_density;
	float  dissolve;
	float2 pad;
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

struct Fog {
	Fog()
		: color(0.4f, 0.4f, 0.4f, 1.0f)
		, start(20.0f)
		, range(50.0f)
	{}
	Fog(float4 color, float start, float range)
		: color(color)
		, start(start)
		, range(range)
	{}
	float4 color;
	float  start;
	float  range;
};

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
	LightBuffer(u32 point_lights, u32 directional_lights, u32 spot_lights,
				float4 fog_color, float fog_start, float fog_range)
		: directional_light_count(directional_lights)
		, point_light_count(point_lights)
		, spot_light_count(spot_lights)
		, pad(0)
		, fog_color(fog_color)
		, fog_start(fog_start)
		, fog_range(fog_range)
		, pad2(0.0f, 0.0f)
	{}
	u32 directional_light_count;
	u32 point_light_count;
	u32 spot_light_count;
	u32 pad;

	float4 fog_color;
	float  fog_start;
	float  fog_range;
	float2 pad2;
};


struct DirectionalLight {
	DirectionalLight()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, pad(0.0f)
	{}
	DirectionalLight(float4 ambient, float4 diffuse, float3 direction, float4 specular)
		: ambient_color(ambient)
		, diffuse_color(diffuse)
		, direction(direction)
		, specular(specular)
		, pad(0.0f)
	{}

	float4 ambient_color;
	float4 diffuse_color;
	float4 specular; //specular.w is the specular power
	float3 direction;
	float  pad;
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
	PointLight(float4 ambient_color, float4 diffuse_color, float4 specular,
			   float3 position, float range, float3 attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, attenuation(attenuation)
		, pad(0.0f)
	{}

	float4 ambient_color;
	float4 diffuse_color;
	float4 specular; //specular.w is the specular power
	float3 position;
	float  range;
	float3 attenuation;
	float  pad;
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
	SpotLight(float4 ambient_color, float4 diffuse_color, float4 specular, float3 position,
	          float range, float3 direction, float spot, float3 attenuation)
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

	float4 ambient_color;
	float4 diffuse_color;
	float4 specular; //specular.w is the specular power
	float3 position;
	float  range;
	float3 direction;
	float  spot;
	float3 attenuation;
	float  pad;
};