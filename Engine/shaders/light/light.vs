#include "shaders\include\light.hlsl"
#include "shaders\include\global.hlsl"


/////////////
// GLOBALS //
/////////////
CONSTANT_BUFFER(MatrixBuffer, MATRIX_BUFFER_SLOT)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

CONSTANT_BUFFER(CameraBuffer, CAMERA_BUFFER_SLOT)
{
	float3 cameraPosition;
	float padding;
}


//////////////
// TYPEDEFS //
//////////////
struct VertexInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
	float3 viewDirection :  TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInput VS(VertexInput input)
{
    PixelInput output;
	float4 world_position;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)world_matrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
	world_position = mul(input.position, world_matrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

    return output;
}