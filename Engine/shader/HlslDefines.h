#ifndef __HLSL_DEFINES__
#define __HLSL_DEFINES__


//----------------------------------------------------------------------------------
// Syntax
//----------------------------------------------------------------------------------

	// Sampler state
	#define REGISTER_S(slot) s##slot
	#define SAMPLER_STATE(name, slot) SamplerState name : register(REGISTER_S(slot))

	// Constant buffer
	#define REGISTER_B(slot) b##slot
	#define CONSTANT_BUFFER(name, slot) cbuffer name : register(REGISTER_B(slot))

	//----------------------------------------------------------------------------------
	// SRVs
	//----------------------------------------------------------------------------------
	#define REGISTER_T(slot) t##slot

		#define TEXTURE_1D(name, slot) Texture1D name : register(REGISTER_T(slot))

		#define TEXTURE_1D_ARRAY(name, slot) Texture1DArray name : register(REGISTER_T(slot))


		#define TEXTURE_2D(name, slot) Texture2D name : register(REGISTER_T(slot))

		#define TEXTURE_2D_ARRAY(name, slot) Texture2DArray name : register(REGISTER_T(slot))


		#define TEXTURE_3D(name, slot) Texture3D name : register(REGISTER_T(slot))



//----------------------------------------------------------------------------------
// Slots
//----------------------------------------------------------------------------------

	// Vertex Shader buffer slots
	#define MATRIX_BUFFER_SLOT 1
	#define CAMERA_BUFFER_SLOT 2

	// Pixel Shader buffer slots
	#define LIGHT_BUFFER_SLOT  1

	// Sampler slots
	#define LINEAR_WRAP_SLOT 1
	#define LINEAR_CLAMP_SLOT 2
	#define POINT_WRAP_SLOT 3
	#define POINT_CLAMP_SLOT 4
	#define ANISO_WRAP_SLOT 5
	#define ANISO_CLAMP_SLOT 6


#endif //__HLSL_DEFINES__