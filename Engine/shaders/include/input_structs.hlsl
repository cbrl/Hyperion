#ifndef HLSL_INPUT_STRUCTS
#define HLSL_INPUT_STRUCTS


//----------------------------------------------------------------------------------
//  Vertex Shader input structs
//----------------------------------------------------------------------------------

struct VSInputPosition {
	float3 position : POSITION0;
};


struct VSInputPositionNormal {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
};


struct VSInputPositionNormalTexture {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 tex      : TEXCOORD0;
};



//----------------------------------------------------------------------------------
//  Pixel Shader input structs
//----------------------------------------------------------------------------------

struct PSInputPosition {
	float4 position : SV_POSITION;
};


struct PSInputPositionNormal {
	float4 position : SV_POSITION;
	float3 normal   : NORMAL0;
};


struct PSInputPositionNormalTexture {
	float4 position  : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal    : NORMAL0;
	float2 tex       : TEXCOORD0;
};


#endif //HLSL_INPUT_STRUCTS