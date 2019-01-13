#include "forward/forward_include.hlsli"

float4 PS(PSPositionNormalTexture pin) : SV_Target {
	return GetBaseColor(pin.uv);
}