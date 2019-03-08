#ifndef HLSL_GLOBAL
#define HLSL_GLOBAL

#include "hlsl.h"
#include "include/syntax.hlsli"
#include "include/input_structs.hlsli"
#include "include/math.hlsli"


//----------------------------------------------------------------------------------
// Sampler States
//----------------------------------------------------------------------------------

SamplerState g_point_wrap   : REG_S(SLOT_SAMPLER_POINT_WRAP);
SamplerState g_point_clamp  : REG_S(SLOT_SAMPLER_POINT_CLAMP);
SamplerState g_linear_wrap  : REG_S(SLOT_SAMPLER_LINEAR_WRAP);
SamplerState g_linear_clamp : REG_S(SLOT_SAMPLER_LINEAR_CLAMP);
SamplerState g_aniso_wrap   : REG_S(SLOT_SAMPLER_ANISO_WRAP);
SamplerState g_aniso_clamp  : REG_S(SLOT_SAMPLER_ANISO_CLAMP);

SamplerComparisonState g_pcf_sampler : REG_S(SLOT_SAMPLER_PCF);



//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

cbuffer EngineBuffer : REG_B(SLOT_CBUFFER_ENGINE) {

	// The screen resolution
	uint2 g_display_resolution;

	// Delta time
	float g_dt;

	// Padding
	float eb_pad0;
}


// Camera fog struct definition
struct Fog {
	float  density;
	float3 color;
};

cbuffer CameraBuffer : REG_B(SLOT_CBUFFER_CAMERA) {

	//----------------------------------------------------------------------------------
	// Matrices
	//----------------------------------------------------------------------------------

	// The camera-to-world matrix
	matrix g_camera_to_world;

	// The world-to-camera (world-to-view) matrix
	matrix g_world_to_camera;

	// The camera-to-projection (view-to-projection) matrix
	matrix g_camera_to_projection;

	// The projection-to-camera (projection-to-view) matrix
	matrix g_projection_to_camera;


	//----------------------------------------------------------------------------------
	// Viewport
	//----------------------------------------------------------------------------------

	// The camera's viewport origin
	uint2 g_viewport_top_left;

	// The camera's viewport resolution
	uint2 g_viewport_resolution;

	// The inverse of the viewport_resolution
	float2 g_inv_viewport_resolution;
	
	// Alignment padding
	float2 g_cbpad0;


	//----------------------------------------------------------------------------------
	// Fog
	//----------------------------------------------------------------------------------

	// The fog the camera sees
	Fog g_fog;
};



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

float3 CameraPosition() {
	return g_camera_to_world._41_42_43;
}

float3 CameraToWorld(float3 p_camera) {
	return mul(float4(p_camera, 1.0f), g_camera_to_world).xyz;
}

float2 DisplayToViewport(float2 p_display) {
	return p_display - g_viewport_top_left;
}

float2 ViewportToUV(float2 p_viewport) {
	return p_viewport * g_inv_viewport_resolution;
}

float3 ClipToCamera(float3 p_clip) {
	const float4 p_projection = {p_clip, 1.0f};
	const float4 p_camera     = mul(p_projection, g_projection_to_camera);
	return Homogenize(p_camera);
}

float3 ViewportToCamera(float2 p_viewport, float depth) {
	const float2 p_clip_uv = UVToClip(ViewportToUV(p_viewport));
	const float3 p_clip    = {p_clip_uv, depth};
	return ClipToCamera(p_clip);
}

float3 DisplayToCamera(float2 p_display, float depth) {
	const float2 p_viewport = DisplayToViewport(p_display);
	return ViewportToCamera(p_viewport, depth);
}


#endif //HLSL_GLOBAL