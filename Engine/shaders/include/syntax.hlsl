#ifndef __HLSL_SYNTAX__
#define __HLSL_SYNTAX__


//----------------------------------------------------------------------------------
// Registers
//----------------------------------------------------------------------------------

#define REG_B(slot) b##slot
#define REG_C(slot) c##slot
#define REG_S(slot) s##slot
#define REG_T(slot) t##slot
#define REG_U(slot) u##slot


//----------------------------------------------------------------------------------
// Sampler State
//----------------------------------------------------------------------------------

#define SAMPLER_STATE(name, slot) SamplerState name : register(REG_S(slot))


//----------------------------------------------------------------------------------
// Buffers
//----------------------------------------------------------------------------------

#define CONSTANT_BUFFER(name, slot) cbuffer name : register(REG_B(slot))
#define STRUCTURED_BUFFER(name, type, slot) StructuredBuffer<type> name : register(REG_T(slot))


//----------------------------------------------------------------------------------
// Textures
//----------------------------------------------------------------------------------

#define TEXTURE_1D(name, slot) Texture1D name : register(REG_T(slot))
#define TEXTURE_1D_ARRAY(name, slot) Texture1DArray name : register(REG_T(slot))

#define TEXTURE_2D(name, slot) Texture2D name : register(REGISTER_T(slot))
#define TEXTURE_2D_ARRAY(name, slot) Texture2DArray name : register(REG_T(slot))

#define TEXTURE_3D(name, slot) Texture3D name : register(REG_T(slot))
#define TEXTURE_CUBE(name, slot) TextureCube name : register(REG_T(slot))
#define TEXTURE_CUBE_ARRAY(name, sloat) TextureCubeArray name : register(REG_T(slot))

#endif //__HLSL_SYNTAX__