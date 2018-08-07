#ifndef HLSL_INPUT_STRUCTS
#define HLSL_INPUT_STRUCTS


//----------------------------------------------------------------------------------
//  Vertex Shader input structs
//----------------------------------------------------------------------------------

struct VSPosition {
	float3 position : POSITION0;
};


struct VSPositionNormal {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
};


struct VSPositionNormalTexture {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 tex      : TEXCOORD0;
};



//----------------------------------------------------------------------------------
//  Pixel Shader input structs
//----------------------------------------------------------------------------------

struct PSWorldPosition {
	float4 position       : SV_POSITION;
	float3 position_world : POSITION0;
};


struct PSTexture {
	float4 position : SV_POSITION;
	float2 tex      : TEXCOORD0;
};


struct PSPositionNormalTexture {
	float4 position       : SV_POSITION;
	float3 position_world : POSITION0;
	float3 normal         : NORMAL0;
	float2 tex            : TEXCOORD0;
};


#endif //HLSL_INPUT_STRUCTS