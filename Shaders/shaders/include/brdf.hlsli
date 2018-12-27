#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/math.hlsli"
#include "include/material.hlsli"


//----------------------------------------------------------------------------------
// DiffuseFactor
//----------------------------------------------------------------------------------
// L: light vector (surface to light)
// N: normal vector
//----------------------------------------------------------------------------------
float DiffuseFactor(float3 L, float3 N) {
	return max(dot(N, L), 0.0f) * g_inv_pi;
}


//----------------------------------------------------------------------------------
// Specular Factor Functions
//----------------------------------------------------------------------------------
//     L: light vector (surface to light)
//     N: normal vector
//     V: view vector (surface to camera)
// power: specular exponent
//----------------------------------------------------------------------------------

float SpecularPhong(float3 L, float3 N, float3 V, float power) {
	const float3 R = normalize(reflect(-L, N));
	const float R_dot_V = max(dot(R, V), 0.0f);
	return pow(R_dot_V, power);
}
float SpecularBlinnPhong(float3 L, float3 N, float3 V, float power) {
	const float3 H = normalize(L + V);
	const float N_dot_H = max(dot(N, H), 0.0f);
	return pow(N_dot_H, power);
}



void ComputeBRDF(float3 p_to_l, float3 n, float3 p_to_v, Material mat,
                 out float3 diffuse, out float3 specular) {

	const float diff_factor = DiffuseFactor(p_to_l, n);
	diffuse  = mat.diffuse.xyz * diff_factor;
	specular = SpecularBlinnPhong(p_to_l, n, p_to_v, mat.spec_exponent);
}


#endif //HLSL_BRDF