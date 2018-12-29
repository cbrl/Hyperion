#ifndef HLSL_DEFINES
#define HLSL_DEFINES


// The maximum alpha value before the material is considered opaque
// 250/255
#define ALPHA_MAX 0.98039215f

// The minimum alpha value before the material is considered invisible
// 1/255
#define ALPHA_MIN 0.00392156f

// The minimum alpha value at which a shadow will still be cast
// 32/255
#define ALPHA_SHADOW_THRESHOLD 0.12549019f


//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

#define SLOT_CBUFFER_ENGINE     1
#define SLOT_CBUFFER_CAMERA     2
#define SLOT_CBUFFER_CAMERA_ALT 3
#define SLOT_CBUFFER_COLOR      4
#define SLOT_CBUFFER_MODEL      5
#define SLOT_CBUFFER_LIGHT      6


//----------------------------------------------------------------------------------
// Sampler slots
//----------------------------------------------------------------------------------

#define SLOT_SAMPLER_LINEAR_WRAP  1
#define SLOT_SAMPLER_LINEAR_CLAMP 2
#define SLOT_SAMPLER_POINT_WRAP   3
#define SLOT_SAMPLER_POINT_CLAMP  4
#define SLOT_SAMPLER_ANISO_WRAP   5
#define SLOT_SAMPLER_ANISO_CLAMP  6
#define SLOT_SAMPLER_PCF          7


//----------------------------------------------------------------------------------
// SRVs
//----------------------------------------------------------------------------------

// Material SRVs
#define SLOT_SRV_BASE_COLOR      0
#define SLOT_SRV_MATERIAL_PARAMS 1
#define SLOT_SRV_NORMAL          2
#define SLOT_SRV_EMISSIVE        3

// Other SRVs
#define SLOT_SRV_SKYBOX          4
#define SLOT_SRV_ENV_MAP         5

// Light
#define SLOT_SRV_DIRECTIONAL_LIGHTS 6
#define SLOT_SRV_POINT_LIGHTS       7
#define SLOT_SRV_SPOT_LIGHTS        8

// Shadowed Lights
#define SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOW 9
#define SLOT_SRV_POINT_LIGHTS_SHADOW       10
#define SLOT_SRV_SPOT_LIGHTS_SHADOW        11

// Shadow Maps
#define SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS 12
#define SLOT_SRV_POINT_LIGHT_SHADOW_MAPS       13
#define SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS        14



#endif //HLSL_DEFINES