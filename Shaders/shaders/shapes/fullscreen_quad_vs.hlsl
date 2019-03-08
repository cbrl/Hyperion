#include "shapes/fullscreen_quad.hlsli"


float4 VS(uint vertex : SV_VertexID) : SV_Position {
	if (vertex < 6)
		return clip_fullscreen_quad[vertex];
	else
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
}