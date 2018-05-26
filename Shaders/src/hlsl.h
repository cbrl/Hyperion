#ifndef HLSL_DEFINES
#define HLSL_DEFINES


//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

#define SLOT_CBUFFER_ENGINE     1
#define SLOT_CBUFFER_CAMERA     2
#define SLOT_CBUFFER_CAMERA_ALT 3
#define SLOT_CBUFFER_LIGHT      4
#define SLOT_CBUFFER_MODEL      5


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

// Material
#define SLOT_SRV_DIFFUSE        0
#define SLOT_SRV_AMBIENT        1
#define SLOT_SRV_NORMAL         2
#define SLOT_SRV_SPECULAR       3
#define SLOT_SRV_SPEC_HIGHLIGHT 4
#define SLOT_SRV_ALPHA          5
#define SLOT_SRV_SKYBOX         6

// Light
#define SLOT_SRV_DIRECTIONAL_LIGHTS 7
#define SLOT_SRV_POINT_LIGHTS       8
#define SLOT_SRV_SPOT_LIGHTS        9

// Shadowed Lights
#define SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOWED 10
#define SLOT_SRV_POINT_LIGHTS_SHADOWED       11
#define SLOT_SRV_SPOT_LIGHTS_SHADOWED        12

// Shadow Maps
#define SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS 13
#define SLOT_SRV_POINT_LIGHT_SHADOW_MAPS       14
#define SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS        15



#endif //HLSL_DEFINES