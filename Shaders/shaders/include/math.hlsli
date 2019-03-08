#ifndef HLSL_MATH
#define HLSL_MATH


//----------------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------------

// Precomputed values of Pi
static const float g_pi            = 3.14159265359;  // pi
static const float g_2pi           = 6.28318530718;  // 2*pi
static const float g_inv_pi        = 0.31830988618;  // 1/pi
static const float g_inv_2pi       = 0.15915494309;  // 1/(2*pi)
static const float g_pi_div_2      = 1.57079632679;  // pi/2
static const float g_pi_div_4      = 0.78539816339;  // pi/4
static const float g_sqrt_2_div_pi = 0.79788456080;  // sqrt(2/pi)


//----------------------------------------------------------------------------------
// Operations - sqr
//----------------------------------------------------------------------------------

int sqr(int x) {
	return x * x;
}
int2 sqr(int2 x) {
	return x * x;
}
int3 sqr(int3 x) {
	return x * x;
}
int4 sqr(int4 x) {
	return x * x;
}


uint sqr(uint x) {
	return x * x;
}
uint2 sqr(uint2 x) {
	return x * x;
}
uint3 sqr(uint3 x) {
	return x * x;
}
uint4 sqr(uint4 x) {
	return x * x;
}


half sqr(half x) {
	return x * x;
}
half2 sqr(half2 x) {
	return x * x;
}
half3 sqr(half3 x) {
	return x * x;
}
half4 sqr(half4 x) {
	return x * x;
}


float sqr(float x) {
	return x * x;
}
float2 sqr(float2 x) {
	return x * x;
}
float3 sqr(float3 x) {
	return x * x;
}
float4 sqr(float4 x) {
	return x * x;
}


double sqr(double x) {
	return x * x;
}
double2 sqr(double2 x) {
	return x * x;
}
double3 sqr(double3 x) {
	return x * x;
}
double4 sqr(double4 x) {
	return x * x;
}


//----------------------------------------------------------------------------------
// Transformations
//----------------------------------------------------------------------------------

half3 Homogenize(half4 input) {
	return input.xyz / input.w;
}

float3 Homogenize(float4 input) {
	return input.xyz / input.w;
}

double3 Homogenize(double4 input) {
	return input.xyz / input.w;
}


//----------------------------------------------------------------------------------
// UNormToSNorm
//----------------------------------------------------------------------------------

float UNormToSNorm(float v_in) {
	return (2.0f * v_in) - 1.0f;
}

float2 UNormToSNorm(float2 v_in) {
	return (2.0f * v_in) - 1.0f;
}

float3 UNormToSNorm(float3 v_in) {
	return (2.0f * v_in) - 1.0f;
}

float4 UNormToSNorm(float4 v_in) {
	return (2.0f * v_in) - 1.0f;
}


//----------------------------------------------------------------------------------
// SNormToUNorm
//----------------------------------------------------------------------------------

float SNormToUNorm(float v_in) {
	return (0.5f * v_in) + 0.5f;
}

float2 SNormToUNorm(float2 v_in) {
	return (0.5f * v_in) + 0.5f;
}

float3 SNormToUNorm(float3 v_in) {
	return (0.5f * v_in) + 0.5f;
}

float4 SNormToUNorm(float4 v_in) {
	return (0.5f * v_in) + 0.5f;
}


//----------------------------------------------------------------------------------
// UV <-> Clip Space
//----------------------------------------------------------------------------------

// {[-1,1], [1,-1]} -> {[0,1], [0,1]}
float2 ClipToUV(float2 p_clip_xy) {
	return (float2(0.5f, -0.5f) * p_clip_xy) + 0.5f;
}

// {[0,1], [0,1]} -> {[-1,1], [1,-1]}
float2 UVToClip(float2 uv) {
	return (float2(2.0f, -2.0f) * uv) + float2(-1.0f, 1.0f);
}


#endif //HLSL_MATH