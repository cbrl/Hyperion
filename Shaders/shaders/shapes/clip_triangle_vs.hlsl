#include "shapes/triangle.hlsli"


float4 VS(uint vertex : SV_VertexID) : SV_Position {
	return ClipSpaceTriangle(vertex);
}