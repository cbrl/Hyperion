#ifndef HLSL_SYNTAX
#define HLSL_SYNTAX

// Simplifies defining buffers, textures, etc. by removing
// the need to manually specify a register.


//----------------------------------------------------------------------------------
// Registers
//----------------------------------------------------------------------------------

#define REG_B(slot) register(b##slot)
#define REG_C(slot) register(c##slot)
#define REG_S(slot) register(s##slot)
#define REG_T(slot) register(t##slot)
#define REG_U(slot) register(u##slot)


//----------------------------------------------------------------------------------
// Sampler State
//----------------------------------------------------------------------------------

#define SAMPLER_STATE(name, slot) SamplerState name : REG_S(slot)
#define SAMPLER_COMPARISON_STATE(name, slot) SamplerComparisonState name : REG_S(slot)


//----------------------------------------------------------------------------------
// Buffers
//----------------------------------------------------------------------------------

#define CONSTANT_BUFFER(name, slot) cbuffer name : REG_B(slot)
#define STRUCTURED_BUFFER(name, type, slot) StructuredBuffer<type> name : REG_T(slot)


//----------------------------------------------------------------------------------
// Textures
//----------------------------------------------------------------------------------

#define TEXTURE_1D(name, slot) Texture1D name : REG_T(slot)
#define TEXTURE_1D_ARRAY(name, slot) Texture1DArray name : REG_T(slot)

#define TEXTURE_2D(name, slot) Texture2D name : REG_T(slot)
#define TEXTURE_2D_ARRAY(name, slot) Texture2DArray name : REG_T(slot)

#define TEXTURE_3D(name, slot) Texture3D name : REG_T(slot)
#define TEXTURE_CUBE(name, slot) TextureCube name : REG_T(slot)
#define TEXTURE_CUBE_ARRAY(name, slot) TextureCubeArray name : REG_T(slot)


#endif //HLSL_SYNTAX