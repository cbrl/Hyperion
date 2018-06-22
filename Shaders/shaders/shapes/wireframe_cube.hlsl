#ifndef HLSL_WIREFRAME_CUBE
#define HLSL_WIREFRAME_CUBE

// D3D11_PRIMITIVE_TOPOLOGY_LINELIST

static const float3 wireframe_cube[24] = {

	{ -0.5, -0.5, -0.5 },
	{ -0.5, -0.5,  0.5 },
	{ -0.5, -0.5, -0.5 },
	{ -0.5,  0.5, -0.5 },
	{ -0.5, -0.5, -0.5 },
	{  0.5, -0.5, -0.5 },
	{ -0.5, -0.5,  0.5 },
	{ -0.5,  0.5,  0.5 },
	{ -0.5, -0.5,  0.5 },
	{  0.5, -0.5,  0.5 },
	{ -0.5,  0.5, -0.5 },
	{ -0.5,  0.5,  0.5 },
	{ -0.5,  0.5, -0.5 },
	{  0.5,  0.5, -0.5 },
	{ -0.5,  0.5,  0.5 },
	{  0.5,  0.5,  0.5 },
	{  0.5, -0.5, -0.5 },
	{  0.5, -0.5,  0.5 },
	{  0.5, -0.5, -0.5 },
	{  0.5,  0.5, -0.5 },
	{  0.5, -0.5,  0.5 },
	{  0.5,  0.5,  0.5 },
	{  0.5,  0.5, -0.5 },
	{  0.5,  0.5,  0.5 }
};


#endif //HLSL_WIREFRAME_CUBE