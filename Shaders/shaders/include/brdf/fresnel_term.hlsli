#ifndef HLSL_BRDF_F_TERM
#define HLSL_BRDF_F_TERM

#include "include/math.hlsli"


//----------------------------------------------------------------------------------
// Fresnel Term Functions
//----------------------------------------------------------------------------------
// l_dot_h: The clamped cos of the angle between the light vector and the half
//          direction between the view and light vector
//      f0: The reflectance parallel to the normal
//  n1, n2: The index of refraction for the two materials (unused in normal rendering)
//----------------------------------------------------------------------------------


namespace BRDF {
namespace Fresnel {

/*
float Schlick_Dielectric_Dielectric(float l_dot_h, float n1, float n2) {
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


float Schlick_Dielectric_Conductor(float l_dot_h, float n1, float n2, float k) {
	// k = imaginary

	// F = [(n1-n2)^2 + 4*n1*n2 * (1-l.h)^5 + k^2] / [(n1 + n2)^2 + k^2]
	// n1 is usually 1 (approx. equal to air)

	const float k_sqr           = -sqr(k);
	const float n1_minus_n2_sqr = sqr(n1 - n2);
	const float base            = 1.0f - l_dot_h;
	const float base_pow_5      = sqr(sqr(base)) * base;

	const float numerator       = n1_minus_n2_sqr + (4.0f * n1 * n2) * base_pow_5 + k_sqr
	const float denominator     = sqr(n1 + n2) + k_sqr;

	return numerator / denominator;
}
*/



//----------------------------------------------------------------------------------
// None
//----------------------------------------------------------------------------------

float None(float l_dot_h, float f0) {
	return f0;
}


float3 None(float l_dot_h, float3 f0) {
	return f0;
}



//----------------------------------------------------------------------------------
// Schlick
//----------------------------------------------------------------------------------

float Schlick(float l_dot_h, float f0, float f90) {
	// F(f0) = f0 + (f90-f0)(1-l.h)^5
	//       = f0(f90 - (1-l.h)^5) + (1-l.h)^5
	//       = lerp(f0, f90, (1-l.h)^5)

	const float base       = 1.0f - l_dot_h;
	const float base_pow_5 = sqr(sqr(base)) * base;

	//return f0 + ((f90 - f0) * base_pow_5);
	return lerp(f0, f90, base_pow_5);
}


float Schlick(float l_dot_h, float f0) {
	return Schlick(l_dot_h, f0, 1.0f);
}


float3 Schlick(float l_dot_h, float3 f0, float3 f90) {
	// F(f0) = f0 + (f90-f0)(1-l.h)^5
	//       = f0(f90 - (1-l.h)^5) + (1-l.h)^5
	//       = lerp(f0, f90, (1-l.h)^5)

	const float base       = 1.0f - l_dot_h;
	const float base_pow_5 = sqr(sqr(base)) * base;

	//return f0 + ((f90 - f0) * base_pow_5);
	return lerp(f0, f90, base_pow_5);
}


float3 Schlick(float l_dot_h, float3 f0) {
	return Schlick(l_dot_h, f0, 1.0f);
}



//----------------------------------------------------------------------------------
// Cook-Torrance
//----------------------------------------------------------------------------------

float CookTorrance(float l_dot_h, float f0) {

	// F = (1/2) * [(g-c) / (g+c)]^2 * { 1 + [((g+c)c - 1)/((g-c)c + 1)]^2 }
	
	// g   = sqrt(eta^2 + c^2 - 1)
	// eta = (1 + sqrt(f0))/(1 - sqrt(f0))
	// c   = l.h

	f0 = min(f0, 0.999f);

	const float f0_sqrt   = sqrt(f0);
	const float eta       = (1.0f + f0_sqrt) / (1.0f - f0_sqrt);
	const float g         = sqrt(sqr(eta) + sqr(l_dot_h) - 1.0f);
	const float g_plus_c  = g + l_dot_h;
	const float g_minus_c = g - l_dot_h;

	const float term1 = sqr(g_minus_c / g_plus_c);
	const float term2 = 1.0f + sqr((g_plus_c * l_dot_h - 1.0f) / (g_minus_c * l_dot_h + 1.0f));

	return 0.5f * term1 * term2;
}


float3 CookTorrance(float l_dot_h, float3 f0) {

	// F = (1/2) * [(g-c) / (g+c)]^2 * { 1 + [((g+c)c - 1)/((g-c)c + 1)]^2 }

	// g   = sqrt(eta^2 + c^2 - 1)
	// eta = (1 + sqrt(f0))/(1 - sqrt(f0))
	// c   = l.h

	f0 = min(f0, 0.999f);

	const float3 f0_sqrt   = sqrt(f0);
	const float3 eta       = (1.0f + f0_sqrt) / (1.0f - f0_sqrt);
	const float3 g         = sqrt(sqr(eta) + sqr(l_dot_h) - 1.0f);
	const float3 g_plus_c  = g + l_dot_h;
	const float3 g_minus_c = g - l_dot_h;

	const float3 term1 = sqr(g_minus_c / g_plus_c);
	const float3 term2 = 1.0f + sqr((g_plus_c * l_dot_h - 1.0f) / (g_minus_c * l_dot_h + 1.0f));

	return 0.5f * term1 * term2;
}

} //namespace Fresnel
} //namespace BRDF


#endif //HLSL_BRDF_F_TERM