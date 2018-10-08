#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	vec4_f32 ambient = {};
	vec4_f32 diffuse = {};
	vec4_f32 specular = {};
	vec4_f32 reflect = {};
	vec4_f32 emissive = {};
	f32 optical_density = {};
	u32 has_texture = {};
	u32 reflection_enabled = {};
};


struct ModelBuffer {
	XMMATRIX       world = XMMatrixIdentity();
	XMMATRIX       world_inv_transpose = XMMatrixIdentity();
	XMMATRIX       texTransform = XMMatrixIdentity();
	MaterialBuffer mat;
};


//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------

struct Fog {
	vec4_f32 color = { 1.0f };
	f32 start = 0.0f;
	f32 range = -1.0f;
	vec2_f32 pad;
};


struct LightBuffer {
	LightBuffer() noexcept
		: num_directional_lights(0)
		, num_point_lights(0)
		, num_spot_lights(0)
		, pad0(0)
		, num_shadow_directional_lights(0)
		, num_shadow_point_lights(0)
		, num_shadow_spot_lights(0)
		, pad1(0)
		, ambient(0.0f, 0.0f, 0.0f, 0.0f) {
	}

	LightBuffer(const u32 directional_lights,
	            const u32 point_lights,
	            const u32 spot_lights,
	            const u32 shadowed_directional_lights,
	            const u32 shadowed_point_lights,
	            const u32 shadowed_spot_lights,
	            const vec4_f32& fog_color,
	            const f32 fog_start,
	            const f32 fog_range,
	            const vec4_f32& ambient_color) noexcept
		: num_directional_lights(directional_lights)
		, num_point_lights(point_lights)
		, num_spot_lights(spot_lights)
		, pad0(0)
		, num_shadow_directional_lights(shadowed_directional_lights)
		, num_shadow_point_lights(shadowed_point_lights)
		, num_shadow_spot_lights(shadowed_spot_lights)
		, pad1(0)
		, ambient(ambient_color) {
	}

	u32 num_directional_lights = 0;
	u32 num_point_lights = 0;
	u32 num_spot_lights = 0;
	u32 pad0 = 0;

	u32 num_shadow_directional_lights = 0;
	u32 num_shadow_point_lights = 0;
	u32 num_shadow_spot_lights = 0;
	u32 pad1 = 0;

	vec4_f32 ambient = {};
};


struct DirectionalLightBuffer {
	vec4_f32 diffuse_color = {};
	vec4_f32 specular = {}; //specular.w is the specular power
	vec3_f32 direction = {};
	u32 pad = {};
	XMMATRIX world_to_projection = XMMatrixIdentity();
};


struct PointLightBuffer {
	vec4_f32 diffuse_color = {};
	vec4_f32 specular = {}; //specular.w is the specular power
	vec3_f32 position = {};
	f32      range = 0;
	vec3_f32 attenuation;
	u32 pad;
};


struct ShadowedPointLightBuffer {
	PointLightBuffer light_buffer;
	XMMATRIX world_to_light = XMMatrixIdentity();
	vec2_f32 projection_values = {};
	vec2_f32 pad = {};
};


struct SpotLightBuffer {
	vec4_f32 diffuse_color = {};
	vec4_f32 specular = {}; //specular.w is the specular power
	vec3_f32 position = {};
	f32      range = 0.0f;
	vec3_f32 direction = {};
	f32      cos_umbra = 0.0f;
	f32      cos_penumbra = 0.0f;
	vec3_f32 attenuation = {};
};


struct ShadowedSpotLightBuffer {
	SpotLightBuffer light_buffer;
	XMMATRIX        world_to_projection = XMMatrixIdentity();
};



//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct CameraBuffer {
	XMMATRIX camera_to_world = XMMatrixIdentity();
	XMMATRIX world_to_camera = XMMatrixIdentity();
	XMMATRIX camera_to_projection = XMMatrixIdentity();
	Fog      fog;
};


struct AltCameraBuffer {
	XMMATRIX world_to_camera = XMMatrixIdentity();
	XMMATRIX camera_to_projection = XMMatrixIdentity();
};