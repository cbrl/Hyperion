#include "include/global.hlsli"

cbuffer Color : REG_B(SLOT_CBUFFER_COLOR) {
	float4 color;
}

float4 PS(float4 position : SV_Position) : SV_Target {
	return color;
}