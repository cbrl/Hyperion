#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/math.hlsli"
#include "include/material.hlsli"


#define BRDF_FUNCTION CookTorranceBRDF

#define DISTRIBUTION_FUNC D_Beckmann
#define FRESNEL_FUNC F_Schlick
#define VISIBILITY_FUNC V_CookTorrance


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
// Distrubution Term Functions
//----------------------------------------------------------------------------------
// n_dot_h: clamped cos of the half angle between the normal and the half direction
//          between the view and light vector
//   alpha: the squared roughness
//----------------------------------------------------------------------------------

float D_BlinnPhong(float n_dot_h, float alpha) {
	// D(h) = K(n.h)^s

	// K     = (s+2) / (2pi)
	// alpha = sqrt(2 / (s+2))
	// s     = (2/(alpha^2)) - 2
	// K     = (1/pi) * (1/(alpha^2))

	const float inv_alpha_sqr = 1.0f / sqr(alpha);
	const float s = (2.0f * inv_alpha_sqr) - 2.0f;
	const float K = g_inv_pi * inv_alpha_sqr;
	return K * pow(n_dot_h, s);
}

float D_Beckmann(float n_dot_h, float alpha) {
	const float inv_alpha_sqr   = 1.0f / sqr(alpha);
	const float n_dot_h_sqr     = sqr(n_dot_h);
	const float inv_n_dot_h_sqr = 1.0f / n_dot_h_sqr;

	const float power = (n_dot_h_sqr - 1) * inv_alpha_sqr * inv_n_dot_h_sqr;

	return exp(power) * g_inv_pi * inv_alpha_sqr * pow(n_dot_h, 4);
}




//----------------------------------------------------------------------------------
// Geometry Term Functions
//----------------------------------------------------------------------------------
// n_dot_l: clamped cos of the angle between the normal and light vector
// n_dot_v: clamped cos of the angle between the normal and view vector
// n_dot_h: clamped cos of the half angle between the normal and the half direction
//          between the view and light vector
//   alpha: the squared roughness
//----------------------------------------------------------------------------------

float G_Implicit(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return n_dot_l * n_dot_v;
}

float G_CookTorrance(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	// G(n,h,v,l) = min(1, (2*n.h*n.v)/(v.h), (2*n.h*n.l)/(v.h))
	//            = min(1, [(2*n.h) / (v.h)] * min(n.v, n.l))

	const float term = (2.0f * n_dot_h) / v_dot_h;
	return min(1.0f, term * min(n_dot_v, n_dot_l));
}

float G1_Schlick(float n_dot_x, float alpha) {
	// G1(n,x) = (n.x) / [(n.x)(1-k) + k]
	// k = alpha * sqrt(2/pi)

	const float k = alpha * g_sqrt_2_div_pi;
	return n_dot_x / ((n_dot_x) * (1.0f - k) + k);
}

float G_SmithSchlick(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return G1_Schlick(n_dot_l, alpha) * G1_Schlick(n_dot_v, alpha);
}




//----------------------------------------------------------------------------------
// Visibility Term Functions
//----------------------------------------------------------------------------------
// n_dot_l: clamped cos of the angle between the normal and light vector
// n_dot_v: clamped cos of the angle between the normal and view vector
// n_dot_h: clamped cos of the half angle between the normal and the half direction
//          between the view and light vector
//   alpha: the squared roughness
//----------------------------------------------------------------------------------

// V Term         = G Term / (n.v * n.l)
// Partial V Term = Partial G Term / n.x

float V_Implicit(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return 1.0f;
}

float V_CookTorrance(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	// V(n,h,v,l) = min(1/(n.v*n.l), (2*n.h)/(v.h*n.l), (2*n.h)/(v.h*n.l))
	//            = min(1/(n.v*n.l), [(2*n.h) / (v.h)] / max(n.v, n.l))

	const float term1 = 1.0f / (n_dot_v * n_dot_l);
	const float term2 = (2.0f * n_dot_h) / v_dot_h;
	return min(term1, term2 / max(n_dot_v, n_dot_l));
}

float V1_Schlick(float n_dot_x, float alpha) {
	// V1(n,x) = 1.0f / [(n.x)(1-k) + k]
	// k = alpha * sqrt(2/pi)

	const float k = alpha * g_sqrt_2_div_pi;
	return 1.0f / (n_dot_x * (1.0f - k) + k);
}

float V_SmithSchlick(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return V1_Schlick(n_dot_l, alpha) * V1_Schlick(n_dot_v, alpha);
}




//----------------------------------------------------------------------------------
// Fresnel Term Functions
//----------------------------------------------------------------------------------
// l_dot_h: The clamped cos of the angle between the light vector and the half
//          direction between the view and light vector
//      f0: The reflectance of a ray parallel to the normal
//  n1, n2: The index of refraction for the two materials (unused in normal rendering)
//----------------------------------------------------------------------------------

/*
float F_Schlick_Dielectric_Dielectric(float l_dot_h, float n1, float n2) {
	// F = [(n1-n2)^2 + 4*n1*n2 * (1-l.h)^5] / (n1 + n2)^2
	//   = f0 + [(4*n1*n2 * (1-l.h)^5) / (n1 + n2)^2]

	// f0 = [(n1-n2)/(n1+n2)]^2
	// n1 is usually 1 (approx. equal to air)

	const float n1_minus_n2_sqr = sqr(n1 - n2);
	const float n1_plus_n2_sqr  = sqr(n1 + n2);
	const float base            = 1.0f - l_dot_h;
	const float base_pow_5      = sqr(sqr(base)) * base;

	const float numerator       = n1_minus_n2_sqr + (4.0f * n1 * n2) * base_pow_5;

	return numerator / n1_plus_n2_sqr;
}

float F_Schlick_Dielectric_Conductor(float l_dot_h, float n1, float n2, float k) {
	// k = imaginary

	// F = [(n1-n2)^2 + 4*n1*n2 * (1-l.h)^5 + k^2] / [(n1 + n2)^2 + k^2]
	//   = f0 + [(4*n1*n2 * (1-l.h)^5) / (n1 + n2)^2]

	// f0 = [(n1-n2)^2 + k^2] / [(n1+n2)^2 + k^2]
	// n1 is usually 1 (approx. equal to air)

	const float k_sqr           = -sqr(k);
	const float n1_minus_n2_sqr = sqr(n1 - n2);
	const float base            = 1.0f - l_dot_h;
	const float base_pow_5      = sqr(sqr(base)) * base;

	const float numerator       = n1_minus_n2_sqr + (4.0f * n1 * n2) * base_pow_5 + k_sqr
	const float denominator     = sqr(n1 + n2) + k_sqr;

	return numerator / denominator;
}

float F_Schlick(float l_dot_h, float n1, float n2) {
	// F(f0) = f0 + (1-f0)(1-l.h)^5
	//       = f0(1 - (1-l.h)^5) + (1-l.h)^5
	//       = lerp(f0, 1, (l.h)^5)

	// f0 = [(n1-n2)/(n1+n2)]^2
	// n1 is usually 1 (approx. equal to air)

	const float f0         = sqr((n1-n2)/(n1+n2));
	const float base       = 1.0f - l_dot_h;
	const float base_pow_5 = sqr(sqr(base)) * base;

	//return f0 + ((1.0f - f0) * base_pow_5);
	return lerp(f0, 1.0f, base_pow_5);
}
*/

float F_Schlick(float l_dot_h, float f0) {
	// F(f0) = f0 + (1-f0)(1-l.h)^5
	//       = f0(1 - (1-l.h)^5) + (1-l.h)^5
	//       = lerp(f0, 1, (l.h)^5)

	const float base       = 1.0f - l_dot_h;
	const float base_pow_5 = sqr(sqr(base)) * base;
	return f0 + ((1.0f - f0) * base_pow_5);

	//return f0 + ((1.0f - f0) * base_pow_5);
	return lerp(f0, 1.0f, base_pow_5);
}

float3 F_Schlick(float l_dot_h, float3 f0) {
	// F(f0) = f0 + (1-f0)(1-l.h)^5
	//       = f0(1 - (1-l.h)^5) + (1-l.h)^5
	//       = lerp(f0, 1, (l.h)^5)

	const float base = 1.0f - l_dot_h;
	const float base_pow_5 = sqr(sqr(base)) * base;
	return f0 + ((1.0f - f0) * base_pow_5);

	//return f0 + ((1.0f - f0) * base_pow_5);
	return lerp(f0, 1.0f, base_pow_5);
}





//----------------------------------------------------------------------------------
// BRDF Functions
//----------------------------------------------------------------------------------
//
// Inputs/Outputs are the same as ConstructBRDF()
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

	const float  n_dot_l = saturate(dot(n, l)); //possibly clamp n.l to [0.001, 1.0]
	const float  n_dot_v = saturate(dot(n, v));
	const float3 h       = normalize(l + v);
	const float  n_dot_h = saturate(dot(n, h));
	const float  l_dot_h = saturate(dot(l, h));
	const float  v_dot_h = saturate(dot(v, h));

	const float  alpha = GetAlpha(mat);
	const float3 f0    = GetF0(mat);

	const float  D = DISTRIBUTION_FUNC(n_dot_h, alpha);
	const float3 F = FRESNEL_FUNC(l_dot_h, f0);
	const float  V = VISIBILITY_FUNC(n_dot_l, n_dot_v, n_dot_h, v_dot_h, alpha);

	diffuse = (1.0f - F) * (1.0f - mat.metalness) * mat.base_color.xyz * g_inv_pi;
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