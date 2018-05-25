#include "shaders\forward\forward_include.hlsl"


PSPositionNormalTexture VS(VSPositionNormalTexture vin) {
	PSPositionNormalTexture vout;

	// Transform to world space
	vout.w_position = mul(float4(vin.position, 1.0f), object_to_world).xyz;

	vout.normal = mul(vin.normal, (float3x3)world_inv_transpose);

	// Interpolating normal can unnormalize it, so normalize it
	vout.normal = normalize(vout.normal);

	// Transform to homogeneous clip space
	const float3 p_view = mul(float4(vout.w_position, 1.0f), world_to_camera).xyz;
	vout.position       = mul(float4(p_view, 1.0f), camera_to_projection);

	// Output vertex attributes for interpolation across triangle
	vout.tex = mul(float4(vin.tex, 0.0f, 1.0f), tex_transform).xy;

	return vout;
}