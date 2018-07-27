#ifndef HLSL_GEOSPHERE
#define HLSL_GEOSPHERE


// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

static const float3 geosphere[384] = {

	{ 0.000000,  0.500000,  0.000000 },
	{ 0.000000,  0.474342, -0.158114 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.000000,  0.474342, -0.158114 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.000000,  0.474342, -0.158114 },
	{ 0.000000,  0.353553, -0.353553 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.474342,  0.000000, -0.158114 },
	{ 0.500000,  0.000000,  0.000000 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.474342,  0.000000, -0.158114 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.474342,  0.000000, -0.158114 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.408248,  0.204124, -0.204124 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.204124,  0.408248, -0.204124 },
	{ 0.000000,  0.353553, -0.353553 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.000000,  0.353553, -0.353553 },
	{ 0.000000,  0.158114, -0.474342 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.204124,  0.204124, -0.408248 },
	{ 0.000000,  0.158114, -0.474342 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.000000,  0.158114, -0.474342 },
	{ 0.000000,  0.000000, -0.500000 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.000000,  0.500000,  0.000000 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.000000,  0.474342,  0.158114 },
	{ 0.000000,  0.474342,  0.158114 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.000000,  0.353553,  0.353553 },
	{ 0.000000,  0.474342,  0.158114 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.158114,  0.474342,  0.000000 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.000000,  0.353553,  0.353553 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.000000,  0.158114,  0.474342 },
	{ 0.000000,  0.158114,  0.474342 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.000000,  0.000000,  0.500000 },
	{ 0.000000,  0.158114,  0.474342 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.000000,  0.353553,  0.353553 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.204124,  0.204124,  0.408248 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.204124,  0.408248,  0.204124 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.353553,  0.353553,  0.000000 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.408248,  0.204124,  0.204124 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.474342,  0.158114,  0.000000 },
	{ 0.500000,  0.000000,  0.000000 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.000000,  0.500000,  0.000000 },
	{ 0.000000,  0.474342,  0.158114 },
	{-0.158114,  0.474342,  0.000000 },
	{-0.158114,  0.474342,  0.000000 },
	{-0.204124,  0.408248,  0.204124 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.158114,  0.474342,  0.000000 },
	{ 0.000000,  0.474342,  0.158114 },
	{-0.204124,  0.408248,  0.204124 },
	{ 0.000000,  0.474342,  0.158114 },
	{ 0.000000,  0.353553,  0.353553 },
	{-0.204124,  0.408248,  0.204124 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.474342,  0.000000,  0.158114 },
	{-0.500000,  0.000000,  0.000000 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.474342,  0.000000,  0.158114 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.474342,  0.000000,  0.158114 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.204124,  0.408248,  0.204124 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.204124,  0.204124,  0.408248 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.408248,  0.204124,  0.204124 },
	{-0.204124,  0.408248,  0.204124 },
	{-0.204124,  0.204124,  0.408248 },
	{-0.204124,  0.408248,  0.204124 },
	{ 0.000000,  0.353553,  0.353553 },
	{-0.204124,  0.204124,  0.408248 },
	{ 0.000000,  0.353553,  0.353553 },
	{ 0.000000,  0.158114,  0.474342 },
	{-0.204124,  0.204124,  0.408248 },
	{-0.204124,  0.204124,  0.408248 },
	{-0.158114,  0.000000,  0.474342 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.204124,  0.204124,  0.408248 },
	{ 0.000000,  0.158114,  0.474342 },
	{-0.158114,  0.000000,  0.474342 },
	{ 0.000000,  0.158114,  0.474342 },
	{ 0.000000,  0.000000,  0.500000 },
	{-0.158114,  0.000000,  0.474342 },
	{ 0.000000,  0.500000,  0.000000 },
	{-0.158114,  0.474342,  0.000000 },
	{ 0.000000,  0.474342, -0.158114 },
	{ 0.000000,  0.474342, -0.158114 },
	{-0.204124,  0.408248, -0.204124 },
	{ 0.000000,  0.353553, -0.353553 },
	{ 0.000000,  0.474342, -0.158114 },
	{-0.158114,  0.474342,  0.000000 },
	{-0.204124,  0.408248, -0.204124 },
	{-0.158114,  0.474342,  0.000000 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.204124,  0.408248, -0.204124 },
	{ 0.000000,  0.353553, -0.353553 },
	{-0.204124,  0.204124, -0.408248 },
	{ 0.000000,  0.158114, -0.474342 },
	{ 0.000000,  0.158114, -0.474342 },
	{-0.158114,  0.000000, -0.474342 },
	{ 0.000000,  0.000000, -0.500000 },
	{ 0.000000,  0.158114, -0.474342 },
	{-0.204124,  0.204124, -0.408248 },
	{-0.158114,  0.000000, -0.474342 },
	{-0.204124,  0.204124, -0.408248 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.158114,  0.000000, -0.474342 },
	{ 0.000000,  0.353553, -0.353553 },
	{-0.204124,  0.408248, -0.204124 },
	{-0.204124,  0.204124, -0.408248 },
	{-0.204124,  0.204124, -0.408248 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.204124,  0.204124, -0.408248 },
	{-0.204124,  0.408248, -0.204124 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.204124,  0.408248, -0.204124 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.353553,  0.353553,  0.000000 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.474342,  0.000000, -0.158114 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.408248,  0.204124, -0.204124 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.474342,  0.000000, -0.158114 },
	{-0.474342,  0.158114,  0.000000 },
	{-0.500000,  0.000000,  0.000000 },
	{-0.474342,  0.000000, -0.158114 },
	{ 0.000000, -0.500000,  0.000000 },
	{ 0.000000, -0.474342, -0.158114 },
	{-0.158114, -0.474342,  0.000000 },
	{-0.158114, -0.474342,  0.000000 },
	{-0.204124, -0.408248, -0.204124 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.158114, -0.474342,  0.000000 },
	{ 0.000000, -0.474342, -0.158114 },
	{-0.204124, -0.408248, -0.204124 },
	{ 0.000000, -0.474342, -0.158114 },
	{ 0.000000, -0.353553, -0.353553 },
	{-0.204124, -0.408248, -0.204124 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.474342,  0.000000, -0.158114 },
	{-0.500000,  0.000000,  0.000000 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.474342,  0.000000, -0.158114 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.474342,  0.000000, -0.158114 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.204124, -0.408248, -0.204124 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.204124, -0.204124, -0.408248 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.408248, -0.204124, -0.204124 },
	{-0.204124, -0.408248, -0.204124 },
	{-0.204124, -0.204124, -0.408248 },
	{-0.204124, -0.408248, -0.204124 },
	{ 0.000000, -0.353553, -0.353553 },
	{-0.204124, -0.204124, -0.408248 },
	{ 0.000000, -0.353553, -0.353553 },
	{ 0.000000, -0.158114, -0.474342 },
	{-0.204124, -0.204124, -0.408248 },
	{-0.204124, -0.204124, -0.408248 },
	{-0.158114,  0.000000, -0.474342 },
	{-0.353553,  0.000000, -0.353553 },
	{-0.204124, -0.204124, -0.408248 },
	{ 0.000000, -0.158114, -0.474342 },
	{-0.158114,  0.000000, -0.474342 },
	{ 0.000000, -0.158114, -0.474342 },
	{ 0.000000,  0.000000, -0.500000 },
	{-0.158114,  0.000000, -0.474342 },
	{ 0.000000, -0.500000,  0.000000 },
	{-0.158114, -0.474342,  0.000000 },
	{ 0.000000, -0.474342,  0.158114 },
	{ 0.000000, -0.474342,  0.158114 },
	{-0.204124, -0.408248,  0.204124 },
	{ 0.000000, -0.353553,  0.353553 },
	{ 0.000000, -0.474342,  0.158114 },
	{-0.158114, -0.474342,  0.000000 },
	{-0.204124, -0.408248,  0.204124 },
	{-0.158114, -0.474342,  0.000000 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.204124, -0.408248,  0.204124 },
	{ 0.000000, -0.353553,  0.353553 },
	{-0.204124, -0.204124,  0.408248 },
	{ 0.000000, -0.158114,  0.474342 },
	{ 0.000000, -0.158114,  0.474342 },
	{-0.158114,  0.000000,  0.474342 },
	{ 0.000000,  0.000000,  0.500000 },
	{ 0.000000, -0.158114,  0.474342 },
	{-0.204124, -0.204124,  0.408248 },
	{-0.158114,  0.000000,  0.474342 },
	{-0.204124, -0.204124,  0.408248 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.158114,  0.000000,  0.474342 },
	{ 0.000000, -0.353553,  0.353553 },
	{-0.204124, -0.408248,  0.204124 },
	{-0.204124, -0.204124,  0.408248 },
	{-0.204124, -0.204124,  0.408248 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.204124, -0.204124,  0.408248 },
	{-0.204124, -0.408248,  0.204124 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.204124, -0.408248,  0.204124 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.353553, -0.353553,  0.000000 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.474342,  0.000000,  0.158114 },
	{-0.353553,  0.000000,  0.353553 },
	{-0.408248, -0.204124,  0.204124 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.474342,  0.000000,  0.158114 },
	{-0.474342, -0.158114,  0.000000 },
	{-0.500000,  0.000000,  0.000000 },
	{-0.474342,  0.000000,  0.158114 },
	{ 0.000000, -0.500000,  0.000000 },
	{ 0.000000, -0.474342,  0.158114 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.000000, -0.474342,  0.158114 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.000000, -0.474342,  0.158114 },
	{ 0.000000, -0.353553,  0.353553 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.500000,  0.000000,  0.000000 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.474342,  0.000000,  0.158114 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.408248, -0.204124,  0.204124 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.204124, -0.408248,  0.204124 },
	{ 0.000000, -0.353553,  0.353553 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.000000, -0.353553,  0.353553 },
	{ 0.000000, -0.158114,  0.474342 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.353553,  0.000000,  0.353553 },
	{ 0.204124, -0.204124,  0.408248 },
	{ 0.000000, -0.158114,  0.474342 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.000000, -0.158114,  0.474342 },
	{ 0.000000,  0.000000,  0.500000 },
	{ 0.158114,  0.000000,  0.474342 },
	{ 0.000000, -0.500000,  0.000000 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.000000, -0.474342, -0.158114 },
	{ 0.000000, -0.474342, -0.158114 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.000000, -0.353553, -0.353553 },
	{ 0.000000, -0.474342, -0.158114 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.158114, -0.474342,  0.000000 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.000000, -0.353553, -0.353553 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.000000, -0.158114, -0.474342 },
	{ 0.000000, -0.158114, -0.474342 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.000000,  0.000000, -0.500000 },
	{ 0.000000, -0.158114, -0.474342 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.158114,  0.000000, -0.474342 },
	{ 0.000000, -0.353553, -0.353553 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.204124, -0.204124, -0.408248 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.204124, -0.408248, -0.204124 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.353553, -0.353553,  0.000000 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.474342,  0.000000, -0.158114 },
	{ 0.353553,  0.000000, -0.353553 },
	{ 0.408248, -0.204124, -0.204124 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.474342,  0.000000, -0.158114 },
	{ 0.474342, -0.158114,  0.000000 },
	{ 0.500000,  0.000000,  0.000000 },
	{ 0.474342,  0.000000, -0.158114 }
};


#endif //HLSL_GEOSPHERE