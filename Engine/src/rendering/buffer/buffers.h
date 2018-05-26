#pragma once

#include "datatypes\datatypes.h"
#include "math\math.h"


//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct CameraBuffer {
	CameraBuffer()
		: position(0.0f, 0.0f, 0.0f)
		, padding(0.0f)
		, world_to_camera(XMMatrixIdentity())
	{}
	CameraBuffer(const float3& pos, CXMMATRIX world_to_camera, CXMMATRIX camera_to_projection)
		: position(pos)
		, world_to_camera(world_to_camera)
		, camera_to_projection(camera_to_projection)
		, padding(0.0f)
	{}
	CameraBuffer(FXMVECTOR pos, CXMMATRIX world_to_camera, CXMMATRIX camera_to_projection)
		: world_to_camera(world_to_camera)
		, camera_to_projection(camera_to_projection)
		, padding(0.0f) {
		XMStoreFloat3(&position, pos);
	}

	float3   position;
	float    padding;
	//XMMATRIX model_view_proj;
	XMMATRIX world_to_camera;
	XMMATRIX camera_to_projection;
};


struct AltCameraBuffer {
	AltCameraBuffer()
		: world_to_camera(XMMatrixIdentity())
		, camera_to_projection(XMMatrixIdentity())
	{}
	AltCameraBuffer(CXMMATRIX world_to_cam, CXMMATRIX cam_to_proj)
		: world_to_camera(world_to_cam)
		, camera_to_projection(cam_to_proj)
	{}

	XMMATRIX world_to_camera;
	XMMATRIX camera_to_projection;
};



//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	MaterialBuffer()
		: ambient(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, reflect(0.0f, 0.0f, 0.0f, 1.0f)
		, emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, optical_density(0.0f)
		, dissolve(0.0f)
		, has_texture(0)
		, reflection_enabled(0)
	{}
	MaterialBuffer(const float4& ambient,
				   const float4& diffuse,
				   const float4& specular,
				   const float4& reflect,
				   const float4& emissive,
				   const float   optical_density,
				   const float   dissolve,
				   const bool    has_texture,
				   const bool    reflection_enabled)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, reflect(reflect)
		, emissive(emissive)
		, optical_density(optical_density)
		, dissolve(dissolve)
		, has_texture(has_texture)
		, reflection_enabled(reflection_enabled)
	{}

	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
	float4 emissive;
	float  optical_density;
	float  dissolve;
	u32    has_texture;
	u32    reflection_enabled;
};

struct ModelBuffer {
	ModelBuffer()
		: world(XMMatrixIdentity())
		, world_inv_transpose(XMMatrixIdentity())
		//, world_view_proj(XMMatrixIdentity())
		, texTransform(XMMatrixIdentity())
	{}

	XMMATRIX world;
	XMMATRIX world_inv_transpose;
	//XMMATRIX world_view_proj;
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
	Fog(const float4& color,
		const float   start,
		const float   range)
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
		: num_point_lights(0)
		, num_directional_lights(0)
		, num_spot_lights(0)
		, pad0(0)
		, num_shadow_directional_lights(0)
		, num_shadow_point_lights(0)
		, num_shadow_spot_lights(0)
		, pad1(0)
		, fog_color(1.0f, 1.0f, 1.0f, 1.0f)
		, fog_start(0.0f)
		, fog_range(0.0f)
		, pad2(0.0f, 0.0f)
	{}
	LightBuffer(const u32     directional_lights,
				const u32     point_lights,
				const u32     spot_lights,
				const u32     shadowed_directional_lights,
				const u32     shadowed_point_lights,
				const u32     shadowed_spot_lights,
				const float4& fog_color,
				const float   fog_start,
				const float   fog_range)
		: num_directional_lights(directional_lights)
		, num_point_lights(point_lights)
		, num_spot_lights(spot_lights)
		, pad0(0)
		, num_shadow_directional_lights(shadowed_directional_lights)
		, num_shadow_point_lights(shadowed_point_lights)
		, num_shadow_spot_lights(shadowed_spot_lights)
		, pad1(0)
		, fog_color(fog_color)
		, fog_start(fog_start)
		, fog_range(fog_range)
		, pad2(0.0f, 0.0f)
	{}

	u32 num_directional_lights;
	u32 num_point_lights;
	u32 num_spot_lights;
	u32 pad0;

	u32 num_shadow_directional_lights;
	u32 num_shadow_point_lights;
	u32 num_shadow_spot_lights;
	u32 pad1;

	float4 fog_color;
	float  fog_start;
	float  fog_range;
	float2 pad2;
};


struct DirectionalLightBuffer {
	DirectionalLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, pad(0.0f)
	{}
	DirectionalLightBuffer(const float4& ambient,
						   const float4& diffuse,
						   const float3& direction,
						   const float4& specular)
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


struct ShadowedDirectionalLightBuffer {
	ShadowedDirectionalLightBuffer()
		: light_buffer()
		, world_to_projection(XMMatrixIdentity())
	{}

	DirectionalLightBuffer light_buffer;
	XMMATRIX world_to_projection;
};


struct PointLightBuffer {
	PointLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 1.0f)
		, range(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
		, pad(0.0f)
	{}
	PointLightBuffer(const float4& ambient_color,
					 const float4& diffuse_color,
					 const float4& specular,
					 const float3& position,
					 const float   range,
					 const float3& attenuation)
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


struct ShadowedPointLightBuffer {
	ShadowedPointLightBuffer()
		: light_buffer()
		, world_to_light(XMMatrixIdentity())
		, projection_values(0.0f, 0.0f)
		, pad(0.0f, 0.0f)
	{}

	PointLightBuffer light_buffer;
	XMMATRIX world_to_light;
	float2 projection_values;
	float2 pad;
};


struct SpotLightBuffer {
	SpotLightBuffer()
		: ambient_color(0.0f, 0.0f, 0.0f, 1.0f)
		, diffuse_color(0.0f, 0.0f, 0.0f, 1.0f)
		, specular(0.0f, 0.0f, 0.0f, 1.0f)
		, position(0.0f, 0.0f, 0.0f)
		, range(0.0f)
		, direction(0.0f, 0.0f, 0.0f)
		, cos_umbra(0.0f)
		, cos_penumbra(0.0f)
		, attenuation(0.0f, 0.0f, 0.0f)
	{}
	SpotLightBuffer(const float4& ambient_color,
					const float4& diffuse_color,
					const float4& specular,
					const float3& position,
					const float   range,
					const float3& direction,
					const float   cos_umbra,
					const float   cos_penumbra,
					const float3& attenuation)
		: ambient_color(ambient_color)
		, diffuse_color(diffuse_color)
		, specular(specular)
		, position(position)
		, range(range)
		, direction(direction)
		, cos_umbra(cos_umbra)
		, cos_penumbra(cos_penumbra)
		, attenuation(attenuation)
	{}

	float4 ambient_color;
	float4 diffuse_color;
	float4 specular; //specular.w is the specular power
	float3 position;
	float  range;
	float3 direction;
	float  cos_umbra;
	float  cos_penumbra;
	float3 attenuation;
};


struct ShadowedSpotLightBuffer {
	ShadowedSpotLightBuffer()
		: light_buffer()
		, world_to_projection(XMMatrixIdentity())
	{}

	SpotLightBuffer light_buffer;
	XMMATRIX world_to_projection;
};