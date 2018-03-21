#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"
#include "shaders\forward\forward.hlsl"


// Included from forward.hlsl
//CONSTANT_BUFFER(Model, SLOT_CBUFFER_MODEL) {
//	matrix world;
//	matrix world_inv_transpose;
//	matrix world_view_proj;
//	matrix texTransform;
//	Material mat;
//};


PSInputPositionNormalTexture VS(VSInputPositionNormalTexture vin) {
	PSInputPositionNormalTexture vout;

	// Transform to world space
	vout.positionW = mul(float4(vin.position, 1.0f), world).xyz;

	vout.normal = mul(vin.normal, (float3x3)world_inv_transpose);
	// Interpolating normal can unnormalize it, so normalize it
	vout.normal = normalize(vout.normal);

	// Transform to homogeneous clip space
	vout.position = mul(float4(vin.position, 1.0f), world_view_proj);

	// Output vertex attributes for interpolation across triangle
	vout.tex = mul(float4(vin.tex, 0.0f, 1.0f), texTransform).xy;

	return vout;
}