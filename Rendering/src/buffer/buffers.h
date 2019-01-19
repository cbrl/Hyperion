#pragma once

#include "datatypes/datatypes.h"
#include "directx/math/directx_math.h"


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	vec4_f32 base_color = {};
	f32      metalness  = 0.0f;
	f32      roughness  = 0.0f;
	vec2_f32 pad0;
	vec3_f32 emissive   = {};
	f32      pad1;
};


struct ModelBuffer {
	XMMATRIX       world               = XMMatrixIdentity();
	XMMATRIX       world_inv_transpose = XMMatrixIdentity();
	XMMATRIX       texTransform        = XMMatrixIdentity();
	MaterialBuffer mat;
};


//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------

struct LightBuffer {
	u32 num_directional_lights = 0;
	u32 num_point_lights       = 0;
	u32 num_spot_lights        = 0;
	f32 pad0;

	u32 num_shadow_directional_lights = 0;
	u32 num_shadow_point_lights       = 0;
	u32 num_shadow_spot_lights        = 0;
	f32 pad1;

	vec4_f32 ambient = {};
};


struct DirectionalLightBuffer {
	vec3_f32 intensity = {};
	f32      pad0;
	vec3_f32 direction = {};
	f32      pad1;
	XMMATRIX world_to_projection = XMMatrixIdentity();
};


struct PointLightBuffer {
	vec3_f32 intensity   = {};
	f32      pad0;
	vec3_f32 position    = {};
	f32      range       = 0;
	vec3_f32 attenuation = {};
	f32      pad1;
};


struct ShadowedPointLightBuffer {
	PointLightBuffer light_buffer;
	XMMATRIX         world_to_light    = XMMatrixIdentity();
	vec2_f32         projection_values = {};
	vec2_f32         pad;
};


struct SpotLightBuffer {
	vec3_f32 intensity    = {};
	f32      pad;
	vec3_f32 position     = {};
	f32      range        = 0.0f;
	vec3_f32 direction    = {};
	f32      cos_umbra    = 0.0f;
	f32      cos_penumbra = 0.0f;
	vec3_f32 attenuation  = {};
};


struct ShadowedSpotLightBuffer {
	SpotLightBuffer light_buffer;
	XMMATRIX world_to_projection = XMMatrixIdentity();
};



//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct Fog {
	f32      density = 0.0f;
	vec3_f32 color   = {1.0f};
};

struct CameraBuffer {
	XMMATRIX camera_to_world      = XMMatrixIdentity();
	XMMATRIX world_to_camera      = XMMatrixIdentity();
	XMMATRIX camera_to_projection = XMMatrixIdentity();
	Fog      fog;
};


struct AltCameraBuffer {
	XMMATRIX world_to_camera      = XMMatrixIdentity();
	XMMATRIX camera_to_projection = XMMatrixIdentity();
};