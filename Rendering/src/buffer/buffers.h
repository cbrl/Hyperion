#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

/*
struct MaterialBuffer {
	vec4_f32 ambient = {};
	vec4_f32 diffuse = {};
	vec4_f32 specular = {};
	vec4_f32 emissive = {};
	f32      spec_scale = 0.0f;
	f32      spec_exponent = 0.0f;
	f32      opacity = 1.0f;
	u32      mirror_surface = 0;
	f32      reflectivity = 1.0f;
	u32      has_texture = 0;
	f32      pad;
};
*/
struct MaterialBuffer {
	vec4_f32 base_color = {};
	f32 metalness = 0.0f;
	f32 roughness = 0.0f;
	vec3_f32 emissive = {};
	vec3_f32 pad;
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

struct LightBuffer {
	u32 num_directional_lights = 0;
	u32 num_point_lights = 0;
	u32 num_spot_lights = 0;
	u32 pad0;

	u32 num_shadow_directional_lights = 0;
	u32 num_shadow_point_lights = 0;
	u32 num_shadow_spot_lights = 0;
	u32 pad1;

	vec4_f32 ambient = {};
};


struct DirectionalLightBuffer {
	vec3_f32 base_color = {};
	f32      intensity = 0.0f;
	vec3_f32 direction = {};
	u32      pad;
	XMMATRIX world_to_projection = XMMatrixIdentity();
};


struct PointLightBuffer {
	vec3_f32 base_color = {};
	f32      intensity = 0.0f;
	vec3_f32 position = {};
	f32      range = 0;
	vec3_f32 attenuation = {};
	u32      pad;
};


struct ShadowedPointLightBuffer {
	PointLightBuffer light_buffer;
	XMMATRIX world_to_light = XMMatrixIdentity();
	vec2_f32 projection_values = {};
	vec2_f32 pad;
};


struct SpotLightBuffer {
	vec3_f32 base_color = {};
	f32      intensity = 0.0f;
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

struct Fog {
	vec4_f32 color = {1.0f};
	f32 start = 0.0f;
	f32 range = -1.0f;
	vec2_f32 pad;
};

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