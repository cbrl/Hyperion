#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/syntax.hlsli"
#include "include/math.hlsli"
#include "include/material.hlsli"

//----------------------------------------------------------------------------------
// Required Defines
//----------------------------------------------------------------------------------
//#define BRDF_FUNCTION ...


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define BRDF_DISTRIBUTION TrowbridgeReitz
#define D_FUNC CAT(CAT(Distribution, ::), BRDF_DISTRIBUTION)

#define BRDF_FRESNEL Schlick
#define F_FUNC CAT(CAT(Fresnel, ::), BRDF_FRESNEL)

#define BRDF_VISIBILITY Smith
#define V_FUNC CAT(CAT(Visibility, ::), BRDF_VISIBILITY)

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
namespace detail {
float3 GetF0(Material mat) {
	// 0.04f = dielectric constant for metalness workflow (IOR of approx. 1.5)
	return lerp(0.04f, mat.base_color.xyz, mat.metalness);
}

float GetAlpha(Material mat) {
	return max(sqr(mat.roughness), 0.01f);
}
} // namespace detail




//----------------------------------------------------------------------------------
// Lambert BRDF
//----------------------------------------------------------------------------------
void Lambert(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {
	diffuse  = (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
	specular = float3(0.0f, 0.0f, 0.0f);
}


//----------------------------------------------------------------------------------
// Blinn-Phong BRDF
//----------------------------------------------------------------------------------
void BlinnPhong(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {
	Lambert(l, n, v, mat, diffuse, specular);

	const float3 h      = normalize(l + v);
	const float n_dot_h = max(dot(n, h), 0.0f);
	const float power   = exp2((1.0f - mat.metalness) * 10.0f); //roughness[1, 0] -> exponent[0, 1024]
	specular = detail::GetF0(mat) * pow(n_dot_h, power);
}


//----------------------------------------------------------------------------------
// Cook-Torrance BRDF
//----------------------------------------------------------------------------------
void CookTorrance(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {

	const float  n_dot_l = saturate(dot(n, l));
	const float  n_dot_v = saturate(dot(n, v));
	const float3 h       = normalize(l + v);
	const float  n_dot_h = saturate(dot(n, h));
	const float  l_dot_h = saturate(dot(l, h));
	const float  v_dot_h = saturate(dot(v, h));

	const float  alpha = detail::GetAlpha(mat);
	const float3 f0    = detail::GetF0(mat);

	const float  D = D_FUNC(n_dot_h, alpha);
	const float3 F = F_FUNC(l_dot_h, f0);
	const float  V = V_FUNC(n_dot_l, n_dot_v, n_dot_h, v_dot_h, alpha);

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

void Compute(float3 l, float3 n, float3 v, Material mat, out float3 diffuse, out float3 specular) {

	BRDF_FUNCTION(l, n, v, mat, diffuse, specular);
}

} //namespace BRDF


#endif //HLSL_BRDF