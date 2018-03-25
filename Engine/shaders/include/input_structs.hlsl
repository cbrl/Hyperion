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

struct PSPosition {
	float4 position   : SV_POSITION;
	float3 w_position : POSITION0;
};


struct PSPositionNormal {
	float4 position : SV_POSITION;
	float3 normal   : NORMAL0;
};


struct PSPositionNormalTexture {
	float4 position   : SV_POSITION;
	float3 w_position : POSITION0;
	float3 normal     : NORMAL0;
	float2 tex        : TEXCOORD0;
};


#endif //HLSL_INPUT_STRUCTS