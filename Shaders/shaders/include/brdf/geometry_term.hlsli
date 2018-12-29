#ifndef HLSL_BRDF_G_TERM
#define HLSL_BRDF_G_TERM

#include "include/math.hlsli"


// Includes Visibility Term Functions
// V Term         = (G Term) / (n.v * n.l)
// Partial V Term = (Partial G Term) / n.x


//----------------------------------------------------------------------------------
// Required Defines
//----------------------------------------------------------------------------------
//#define G1_FUNC G1_...
//#define V1_FUNC V1_...




//----------------------------------------------------------------------------------
// Geometry Term Functions
//----------------------------------------------------------------------------------
// n_dot_l: clamped cos of the angle between the normal and light vector
// n_dot_v: clamped cos of the angle between the normal and view vector
// n_dot_h: clamped cos of the half angle between the normal and the half direction
//          between the view and light vector
//   alpha: the squared roughness
// n_dot_x: n.v or n.l (used in partial term functions)
//----------------------------------------------------------------------------------

float G_Implicit(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return n_dot_l * n_dot_v;
}


float G_Neumann(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return (n_dot_l * n_dot_v) / max(n_dot_l, n_dot_v);
}


float G_CookTorrance(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	// G(n,h,v,l) = min( 1, (2*n.h*n.v)/(v.h), (2*n.h*n.l)/(v.h) )
	//            = min( 1, [(2*n.h) / (v.h)] * min(n.v, n.l) )

	const float term = (2.0f * n_dot_h) / v_dot_h;
	return min(1.0f, term * min(n_dot_v, n_dot_l));
}


float G_Kelemen(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return (n_dot_l * n_dot_v) / sqr(v_dot_h);
}


//----------------------------------------------------------------------------------
// Partial Geometry Term Functions (Smith Method)
//----------------------------------------------------------------------------------

float G1_Schlick(float n_dot_x, float alpha) {
	// G1(n,x) = (n.x) / [(n.x)(1-k) + k]
	// k = alpha * sqrt(2/pi)

	const float k = alpha * g_sqrt_2_div_pi;
	return n_dot_x / ((n_dot_x) * (1.0f - k) + k);
}


float G1_Beckmann(float n_dot_x, float alpha) {
	const float c     = n_dot_x / (alpha * sqrt(1.0f - sqr(n_dot_x)));
	const float c_sqr = sqr(c);

	if (c < 1.6f) {
		return ((3.535f * c) + (2.181f * c_sqr)) / (1.0f + (2.276 * c) + (2.577 * c_sqr));
	}
	else {
		return 1.0f;
	}
}


float G1_GGX(float n_dot_x, float alpha) {

	// G1 = 2(n.x) / [(n.x) + sqrt(alpha^2 + (1-alpha^2)(n.x)^2)]

	const float alpha_sqr   = sqr(alpha);
	const float denominator = n_dot_x + sqrt(alpha_sqr + (1.0f - alpha_sqr) * sqr(n_dot_x));

	return (2.0f * n_dot_x) / denominator;
}


float G_Smith(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return G1_FUNC(n_dot_l, alpha) * G1_FUNC(n_dot_v, alpha);
}




//----------------------------------------------------------------------------------
// Visibility Term Functions
//----------------------------------------------------------------------------------
// n_dot_l: clamped cos of the angle between the normal and light vector
// n_dot_v: clamped cos of the angle between the normal and view vector
// n_dot_h: clamped cos of the half angle between the normal and the half direction
//          between the view and light vector
//   alpha: the squared roughness
// n_dot_x: n.v or n.l (used in partial term functions)
//----------------------------------------------------------------------------------

float V_Implicit(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return 1.0f;
}


float V_Neumann(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return 1.0f / max(n_dot_l, n_dot_v);
}


float V_CookTorrance(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	// V(n,h,v,l) = min( 1/(n.v*n.l), (2*n.h)/(v.h*n.l), (2*n.h)/(v.h*n.l) )
	//            = min( 1/(n.v*n.l), [(2*n.h) / (v.h)] / max(n.v, n.l) )

	const float term1 = 1.0f / (n_dot_v * n_dot_l);
	const float term2 = (2.0f * n_dot_h) / v_dot_h;
	return min(term1, term2 / max(n_dot_v, n_dot_l));
}


float V_Kelemen(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return 1.0f / sqr(v_dot_h);
}


//----------------------------------------------------------------------------------
// Partial Visibility Term Functions (Smith Method)
//----------------------------------------------------------------------------------

float V1_Schlick(float n_dot_x, float alpha) {

	// V1(n,x) = 1.0f / [(n.x)(1-k) + k]
	// k = alpha * sqrt(2/pi)

	const float k = alpha * g_sqrt_2_div_pi;
	return 1.0f / (n_dot_x * (1.0f - k) + k);
}


float V1_GGX(float n_dot_x, float alpha) {

	// V1 = 2 / [(n.x) + sqrt(alpha^2 + (1-alpha^2)(n.x)^2)]

	const float alpha_sqr   = sqr(alpha);
	const float denominator = n_dot_x + sqrt(alpha_sqr + (1.0f - alpha_sqr) * sqr(n_dot_x));

	return 2.0f / denominator;
}


float V_Smith(float n_dot_l, float n_dot_v, float n_dot_h, float v_dot_h, float alpha) {
	return V1_FUNC(n_dot_l, alpha) * V1_FUNC(n_dot_v, alpha);
}



#endif //HLSL_BRDF_G_TERM