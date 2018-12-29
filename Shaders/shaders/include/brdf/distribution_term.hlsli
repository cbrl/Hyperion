#ifndef HLSL_BRDF_D_TERM
#define HLSL_BRDF_D_TERM

#include "include/math.hlsli"


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
	const float s             = (2.0f * inv_alpha_sqr) - 2.0f;
	const float K             = g_inv_pi * inv_alpha_sqr;
	return K * pow(n_dot_h, s);
}


float D_Beckmann(float n_dot_h, float alpha) {

	// D = [1 / (pi * alpha^2 * (n.h)^4)] * e^[((n.h)^2 - 1) / (alpha^2 * (n.h)^2)]

	const float inv_alpha_sqr   = 1.0f / sqr(alpha);
	const float n_dot_h_sqr     = sqr(n_dot_h);
	const float inv_n_dot_h_sqr = 1.0f / n_dot_h_sqr;

	const float power = (n_dot_h_sqr - 1) * inv_alpha_sqr * inv_n_dot_h_sqr;

	return exp(power) * g_inv_pi * inv_alpha_sqr * sqr(inv_n_dot_h_sqr);
}


// AKA: GGX
float D_TrowbridgeReitz(float n_dot_h, float alpha) {
	// D = alpha^2 / { pi * [(n.h)^2 * (alpha^2 - 1) + 1]^2 }

	const float alpha_sqr  = sqr(alpha);
	const float denom_term = sqr(n_dot_h) * (alpha_sqr - 1.0f) + 1;

	return (alpha_sqr / sqr(denom_term)) * g_inv_pi;
}


// Anisotropic GGX (Generalized Trowbridge Reitz)
float D_GTR2_Anisotropic(float n_dot_h, float x_dot_h, float y_dot_h, float alpha_x, float alpha_y) {

	// D = (1/pi) * [1 / (a_x * a_y)] * { 1 / [(x.h)^2/a_x^2 + (y.h)^2/a_y^2 + n.h^2]^2 }

	const float inv_alpha_x = 1.0f / alpha_x;
	const float inv_alpha_y = 1.0f / alpha_y;

	//float denominator = (sqr(x_dot_h) * sqr(inv_alpha_x)) + (sqr(y_dot_h) * sqr(inv_alpha_y)) + sqr(n_dot_h);
	//denominator = sqr(denominator);

	const float3 terms       = float3(x_dot_h * inv_alpha_x, y_dot_h * inv_alpha_y, n_dot_h);
	const float  denominator = dot(terms, terms);

	return (g_inv_pi * inv_alpha_x * inv_alpha_y) / denominator;
}


#endif //HLSL_BRDF_D_TERM