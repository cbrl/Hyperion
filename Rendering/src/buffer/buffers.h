#pragma once

#include "datatypes/datatypes.h"
#include "directxmath/directxmath.h"


namespace render {

//----------------------------------------------------------------------------------
// Engine Buffer
//----------------------------------------------------------------------------------
struct EngineBuffer {
	u32_2 resolution;
	f32   delta_time;
	f32   pad0;
};


//----------------------------------------------------------------------------------
// Model Buffer
//----------------------------------------------------------------------------------

struct MaterialBuffer {
	f32_4 base_color = {};
	f32   roughness  = 0.0f;
	f32   metalness  = 0.0f;
	f32_2 pad0;
	f32_3 emissive   = {};
	f32   pad1;
};


struct ModelBuffer {
	XMMATRIX       world               = XMMatrixIdentity();
	XMMATRIX       world_inv_transpose = XMMatrixIdentity();
	XMMATRIX       tex_transform       = XMMatrixIdentity();
	MaterialBuffer mat;
};


//----------------------------------------------------------------------------------
// Light Buffers
//----------------------------------------------------------------------------------

struct LightBuffer {
	f32_3 ambient = {};
	f32   pad0;
};


struct DirectionalLightBuffer {
	f32_3 intensity = {};
	f32   pad0;
	f32_3 direction = {};
	f32   pad1;
	XMMATRIX world_to_projection = XMMatrixIdentity();
};


struct PointLightBuffer {
	f32_3 intensity   = {};
	f32   pad0;
	f32_3 position    = {};
	f32   range       = 0;
	f32_3 attenuation = {};
	f32   pad1;
};


struct ShadowedPointLightBuffer {
	PointLightBuffer light_buffer;
	XMMATRIX         world_to_light    = XMMatrixIdentity();
	f32_2            projection_values = {};
	f32_2            pad;
};


struct SpotLightBuffer {
	f32_3 intensity    = {};
	f32   pad;
	f32_3 position     = {};
	f32   range        = 0.0f;
	f32_3 direction    = {};
	f32   cos_umbra    = 0.0f;
	f32   cos_penumbra = 0.0f;
	f32_3 attenuation  = {};
};


struct ShadowedSpotLightBuffer {
	SpotLightBuffer light_buffer;
	XMMATRIX world_to_projection = XMMatrixIdentity();
};



//----------------------------------------------------------------------------------
// Camera Buffer
//----------------------------------------------------------------------------------

struct FogBuffer {
	f32   density = 0.0f;
	f32_3 color   = {1.0f};
};

struct CameraBuffer {
	XMMATRIX  camera_to_world         = XMMatrixIdentity();
	XMMATRIX  world_to_camera         = XMMatrixIdentity();
	XMMATRIX  camera_to_projection    = XMMatrixIdentity();
	XMMATRIX  projection_to_camera    = XMMatrixIdentity();
	u32_2     viewport_top_left       = {};
	u32_2     viewport_resolution     = {};
	f32_2     inv_viewport_resolution = {};
	f32_2     pad0;
	FogBuffer fog;
};


struct AltCameraBuffer {
	XMMATRIX world_to_camera      = XMMatrixIdentity();
	XMMATRIX camera_to_projection = XMMatrixIdentity();
};

} // namespace render
