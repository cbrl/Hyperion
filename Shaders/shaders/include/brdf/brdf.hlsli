#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/math.hlsli"
#include "include/material.hlsli"

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define BRDF_FUNCTION CookTorranceBRDF

#define DISTRIBUTION_FUNC D_TrowbridgeReitz
#define FRESNEL_FUNC F_None
#define VISIBILITY_FUNC V_Implicit

#define G1_FUNC G1_GGX
#define V1_FUNC V1_GGX


//----------------------------------------------------------------------------------
// Cook-Torrance BRDF Terms
//----------------------------------------------------------------------------------
#include "distribution_term.hlsli"
#include "geometry_term.hlsli"
#include "fresnel_term.hlsli"


//----------------------------------------------------------------------------------
// Utility Functions
//----------------------------------------------------------------------------------

float3 GetF0(Material mat) {
	// 0.04f = dielectric constant for metalness workflow (IOR of approx. 1.5)
	return lerp(0.04f, mat.base_color.xyz, mat.metalness);
}

float GetAlpha(Material mat) {
	return max(sqr(mat.roughness), 0.01f);
}




//----------------------------------------------------------------------------------
// Naive Specular Functions
//----------------------------------------------------------------------------------
float SpecularPhong(float3 l, float3 n, float3 v, float power) {
	const float3 r = normalize(reflect(-l, n));
	const float r_dot_v = max(dot(r, v), 0.0f);
	return pow(r_dot_v, power);
}
float SpecularBlinnPhong(float3 l, float3 n, float3 v, float power) {
	const float3 h = normalize(l + v);
	const float n_dot_h = max(dot(n, h), 0.0f);
	return pow(n_dot_h, power);
}




//----------------------------------------------------------------------------------
// BRDF Functions
//----------------------------------------------------------------------------------
//
// Inputs/Outputs are the same as ComputeBRDF()
//
//----------------------------------------------------------------------------------

void LambertBRDF(float3 l, float3 n, float3 v, Material mat,
                 out float3 diffuse, out float3 specular) {

	const float n_dot_l = saturate(dot(n, l));
	diffuse  = n_dot_l * (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
	specular = float3(0.0f, 0.0f, 0.0f);
}

/*
void BlinnPhongBRDF(float3 l, float3 n, float3 v, Material mat,
                    out float3 diffuse, out float3 specular) {

	const float n_dot_l = saturate(dot(n, l));
	diffuse  = n_dot_l * (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
	specular = SpecularBlinnPhong(l, n, v, mat.spec_exponent) * mat.specular.xyz * mat.spec_scale;
}
*/


void CookTorranceBRDF(float3 l, float3 n, float3 v, Material mat,
                      out float3 diffuse, out float3 specular) {

	const float  n_dot_l = saturate(dot(n, l)); //possibly clamp to [0.001, 1.0]
	const float  n_dot_v = saturate(dot(n, v)); //possibly clamp to [0.001, 1.0]
	const float3 h       = normalize(l + v);
	const float  n_dot_h = saturate(dot(n, h));
	const float  l_dot_h = saturate(dot(l, h));
	const float  v_dot_h = saturate(dot(v, h));

	const float  alpha = GetAlpha(mat);
	const float3 f0    = GetF0(mat);

	const float  D = DISTRIBUTION_FUNC(n_dot_h, alpha);
	const float3 F = FRESNEL_FUNC(l_dot_h, f0);
	const float  V = VISIBILITY_FUNC(n_dot_l, n_dot_v, n_dot_h, v_dot_h, alpha);

	diffuse  = (1.0f - F) * (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
	specular = D * F * V * 0.25f;
}




//----------------------------------------------------------------------------------
// Compute BRDF
//----------------------------------------------------------------------------------
// Inputs
//     l: light vector (surface to light)
//     n: normal vector
//     v: view vector (surface to camera)
//   mat: the model's material
//
// Outputs
//    diffuse: The diffuse intensity
//   specular: The specular intensity
//----------------------------------------------------------------------------------

void ComputeBRDF(float3 l, float3 n, float3 v, Material mat,
                 out float3 diffuse, out float3 specular) {

	BRDF_FUNCTION(l, n, v, mat, diffuse, specular);
}


#endif //HLSL_BRDF