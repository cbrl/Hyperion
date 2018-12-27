#ifndef HLSL_BRDF
#define HLSL_BRDF

#include "include/math.hlsli"
#include "include/material.hlsli"


#define BRDF_FUNCTION BlinnPhongBRDF


//----------------------------------------------------------------------------------
// Lambert BRDF
//----------------------------------------------------------------------------------
// l: light vector (surface to light)
// n: normal vector
// v: 
//----------------------------------------------------------------------------------

float DiffuseLambert(float3 l, float3 n) {
	return max(dot(n, l), 0.0f) * g_inv_pi;
}

void LambertBRDF(float3 l, float3 n, float3 v, Material mat,
                 out float3 diffuse, out float3 specular) {

	diffuse  = DiffuseLambert(l, n) * mat.diffuse.xyz;
	specular = float3(0.0f, 0.0f, 0.0f);
}





//----------------------------------------------------------------------------------
// Blinn(-Phong) BRDF
//----------------------------------------------------------------------------------
//     l: light vector (surface to light)
//     n: normal vector
//     v: view vector (surface to camera)
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

void PhongBRDF(float3 l, float3 n, float3 v, Material mat,
               out float3 diffuse, out float3 specular) {

	const float n_dot_l = dot(n, l);
	diffuse  = DiffuseLambert(l, n) * mat.diffuse.xyz;
	specular = SpecularPhong(l, n, v, mat.spec_exponent) * mat.specular.xyz * mat.spec_scale;
}

void BlinnPhongBRDF(float3 l, float3 n, float3 v, Material mat,
                    out float3 diffuse, out float3 specular) {

	const float n_dot_l = dot(n, l);
	diffuse  = DiffuseLambert(l, n) * mat.diffuse.xyz;
	specular = SpecularBlinnPhong(l, n, v, mat.spec_exponent) * mat.specular.xyz * mat.spec_scale;
}




//----------------------------------------------------------------------------------
// Use the defined BRDF function to calculate the lighting
//----------------------------------------------------------------------------------

void ComputeBRDF(float3 l, float3 n, float3 v, Material mat,
                 out float3 diffuse, out float3 specular) {

	BRDF_FUNCTION(l, n, v, mat, diffuse, specular);
}


#endif //HLSL_BRDF