#ifndef HLSL_INPUT_STRUCTS
#define HLSL_INPUT_STRUCTS


//----------------------------------------------------------------------------------
//  Vertex Shader input structs
//----------------------------------------------------------------------------------

struct VSPosition {
	float3 p : POSITION0;
};


struct VSPositionNormal {
	float3 p : POSITION0;
	float3 n   : NORMAL0;
};


struct VSPositionNormalTexture {
	float3 p  : POSITION0;
	float3 n  : NORMAL0;
	float2 uv : TEXCOORD0;
};



//----------------------------------------------------------------------------------
//  Pixel Shader input structs
//----------------------------------------------------------------------------------

struct PSWorldPosition {
	float4 p       : SV_POSITION;
	float3 p_world : POSITION0;
};


struct PSPositionNormal {
	float4 p : SV_POSITION;
	float2 n : NORMAL0;
};


struct PSPositionTexture {
	float4 p  : SV_POSITION;
	float2 uv : POSITION0;
};


struct PSPositionNormalTexture {
	float4 p       : SV_POSITION;
	float3 p_world : POSITION0;
	float3 n       : NORMAL0;
	float2 uv      : TEXCOORD0;
};


#endif //HLSL_INPUT_STRUCTS