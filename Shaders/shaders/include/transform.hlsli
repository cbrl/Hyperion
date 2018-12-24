#ifndef HLSL_TRANSFORM
#define HLSL_TRANSFORM

#include "include/input_structs.hlsli"


// Transform a texture
float2 Transform(float2 tex, matrix tex_transform) {
	return mul(float4(tex, 0.0f, 1.0f), tex_transform).xy;
}


// Transform a point from object space to projection space
float4 Transform(float3 position,
				 matrix object_to_world,
				 matrix world_to_view,
				 matrix view_to_projection) {

	float3 world = mul(float4(position, 1.0f), object_to_world).xyz;
	float3 view  = mul(float4(world, 1.0f), world_to_view).xyz;
	float4 proj  = mul(float4(view, 1.0f), view_to_projection);

	return proj;
}


// Transform a point from world space to projection space
float4 Transform(float3 world,
				 matrix world_to_view,
				 matrix view_to_projection) {

	const float3 view = mul(float4(world, 1.0f), world_to_view).xyz;
	const float4 proj = mul(float4(view, 1.0f), view_to_projection);

	return proj;
}


PSPositionNormalTexture Transform(VSPositionNormalTexture vin,
								  matrix object_to_world,
								  matrix world_to_view,
								  matrix view_to_projection,
								  matrix world_inv_transpose,
								  matrix texture_transform) {

	PSPositionNormalTexture vout;

	// Transform to world space
	vout.p_world = mul(float4(vin.p, 1.0f), object_to_world).xyz;

	vout.n = normalize(mul(vin.n, (float3x3)world_inv_transpose));

	// Transform to homogeneous clip space
	const float3 p_view = mul(float4(vout.p_world, 1.0f), world_to_view).xyz;
	vout.p       = mul(float4(p_view, 1.0f), view_to_projection);

	// Output vertex attributes for interpolation across triangle
	vout.uv = mul(float4(vin.uv, 0.0f, 1.0f), texture_transform).xy;

	return vout;
}



#endif //HLSL_TRANSFORM