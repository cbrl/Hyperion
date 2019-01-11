#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/syntax.hlsli"
#include "include/math.hlsli"
#include "include/material.hlsli"

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define BRDF_FUNCTION CookTorrance

#define D_FUNC Beckmann
#define DISTRIBUTION CAT(CAT(Distribution, ::), D_FUNC)

#define F_FUNC CookTorrance
#define FRESNEL CAT(CAT(Fresnel, ::), F_FUNC)

#define V_FUNC Smith
#define VISIBILITY CAT(CAT(Visibility, ::), V_FUNC)

#define G1_FUNC G1_GGX
#define V1_FUNC V1_GGX


//----------------------------------------------------------------------------------
// Cook-Torrance BRDF Terms
//----------------------------------------------------------------------------------
#include "distribution_term.hlsli"
#include "geometry_term.hlsli"
#include "fresnel_term.hlsli"


namespace BRDF {

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
	const float3 r       = normalize(reflect(-l, n));
	const float  r_dot_v = max(dot(r, v), 0.0f);
	return pow(r_dot_v, power);
}
float SpecularBlinnPhong(float3 l, float3 n, float3 v, float power) {
	const float3 h       = normalize(l + v);
	const float  n_dot_h = max(dot(n, h), 0.0f);
	return pow(n_dot_h, power);
}




//----------------------------------------------------------------------------------
// BRDF Functions
//----------------------------------------------------------------------------------
//
// Inputs/Outputs are the same as ComputeBRDF()
//
//----------------------------------------------------------------------------------

void Lambert(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {
	diffuse  = (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
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


void CookTorrance(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {

	const float  n_dot_l = saturate(dot(n, l));
	const float  n_dot_v = saturate(dot(n, v));
	const float3 h       = normalize(l + v);
	const float  n_dot_h = saturate(dot(n, h));
	const float  l_dot_h = saturate(dot(l, h));
	const float  v_dot_h = saturate(dot(v, h));

	const float  alpha = GetAlpha(mat);
	const float3 f0    = GetF0(mat);

	const float  D = DISTRIBUTION(n_dot_h, alpha);
	const float3 F = FRESNEL(l_dot_h, f0);
	const float  V = VISIBILITY(n_dot_l, n_dot_v, n_dot_h, v_dot_h, alpha);

	diffuse = (1.0f - F) * (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
	diffuse = saturate(diffuse);

	specular = D * F * V * 0.25f;
	specular = saturate(specular);
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

void ComputeBRDF(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {

	BRDF_FUNCTION(l, n, v, mat, diffuse, specular);
}

} //namespace BRDF


#endif //HLSL_BRDF