#ifndef __HLSL_DEFINES__
#define __HLSL_DEFINES__

//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

#define SLOT_CBUFFER_MATRIX 1
#define SLOT_CBUFFER_CAMERA 2
//#define SLOT_CBUFFER_LIGHT  3


//----------------------------------------------------------------------------------
// Sampler slots
//----------------------------------------------------------------------------------

#define SLOT_SAMPLER_LINEAR_WRAP  1
#define SLOT_SAMPLER_LINEAR_CLAMP 2
#define SLOT_SAMPLER_POINT_WRAP   3
#define SLOT_SAMPLER_POINT_CLAMP  4
#define SLOT_SAMPLER_ANISO_WRAP   5
#define SLOT_SAMPLER_ANISO_CLAMP  6


//----------------------------------------------------------------------------------
// SRVs
//----------------------------------------------------------------------------------

// Material
#define SLOT_SRV_DIFFUSE  1
#define SLOT_SRV_AMBIENT  2
#define SLOT_SRV_SPECULAR 3
#define SLOT_SRV_ALPHA    4
#define SLOT_SRV_BUMP     5

// Light
#define SLOT_SRV_POINT_LIGHTS       6
#define SLOT_SRV_SPOT_LIGHTS        7
#define SLOT_SRV_DIRECTIONAL_LIGHTS 8


#endif //__HLSL_DEFINES__